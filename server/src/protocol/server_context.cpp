#include "tds/protocol/server_context.hpp"

#include "tds/protocol/protocol_error.hpp"

#include <algorithm>

namespace fs = std::filesystem;

namespace tds::protocol {
    ServerContext::ServerContext(fs::path root, const config::ServerConfig& server_config)
        : m_root{std::move(root)}
        , m_config_directory{m_root / ".tds"}
        , m_upload_max_size{server_config.get_upload_max_size()} {
        m_user_table.open(get_config_directory_path() / "users");
    }

    const fs::path& ServerContext::get_root_path() const noexcept {
        return m_root;
    }

    const fs::path& ServerContext::get_config_directory_path() const noexcept {
        return m_config_directory;
    }

    fs::path ServerContext::get_relative_path_to(const fs::path& subpath) const {
        fs::path result = fs::relative(subpath, get_root_path());
        if(result == ".") {
            result.clear();
        }
        return fs::path{"/"} / std::move(result);
    }

    std::shared_ptr<AuthToken> ServerContext::authorize_user(std::string_view username, const std::string& password) {
        std::scoped_lock lock{m_auth_mutex};
        remove_expired_auth_tokens();

        if(!m_user_table.has_user(username) || !m_user_table.verify_password_of_user(username, password)) {
            throw ProtocolError{ProtocolCode::invalid_credentials};
        }

        const bool already_authorized = std::ranges::find(m_auth_tokens, username, [](auto& ptr) {
                                            return ptr.lock()->get_username();
                                        }) != m_auth_tokens.end();
        if(already_authorized) {
            throw ProtocolError{ProtocolCode::user_already_logged};
        }

        auto auth_token = std::make_shared<AuthToken>(std::string{username}, m_user_table.get_perms_of_user(username));
        m_auth_tokens.emplace_back(auth_token);
        return auth_token;
    }

    std::shared_ptr<PathLock> ServerContext::lock_path(const std::filesystem::path& path) {
        throw_on_invalid_path(path);
        auto path_lock = make_path_lock(path);
        insert_path_lock(path_lock);
        return path_lock;
    }

    std::shared_ptr<DownloadToken> ServerContext::download_file(const std::filesystem::path& path) {
        throw_on_invalid_path(path);
        auto download_lock = make_download_token(path);
        insert_path_lock(download_lock);
        return download_lock;
    }

    std::shared_ptr<UploadToken> ServerContext::upload_file(const std::filesystem::path& path, std::uintmax_t size) {
        if(!path.is_absolute()) {
            throw ProtocolError{ProtocolCode::invalid_file_type};
        } else if(fs::exists(path)) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        } else if(!fs::exists(path.parent_path())) {
            throw ProtocolError{ProtocolCode::not_found};
        }

        const auto space_info = fs::space(get_root_path());
        if(size > m_upload_max_size || size > space_info.available) {
            throw ProtocolError{ProtocolCode::too_large_file};
        }

        throw_if_file_is_already_uploaded(path);
        auto upload_lock = make_upload_token(path, size);
        insert_path_lock(upload_lock);
        return upload_lock;
    }

    bool ServerContext::is_path_forbidden(const std::filesystem::path& path) const {
        if(!path.is_absolute()) {
            throw ProtocolError{ProtocolCode::unknown,
                                "Server context requires absolute paths. Please send this to server developer."};
        } else {
            return path.native().starts_with(get_config_directory_path().native());
        }
    }

    bool ServerContext::is_path_locked(const std::filesystem::path& path) const {
        auto is_locking_path = [&path](auto& lock_ptr) {
            const std::shared_ptr<const PathLock> lock_shared_ptr = lock_ptr.lock();
            return lock_shared_ptr != nullptr && lock_shared_ptr->has_locked_path(path);
        };

        std::shared_lock lock{m_path_locks_mutex};
        return std::ranges::find_if(m_path_locks, is_locking_path) != m_path_locks.end();
    }

    std::string ServerContext::get_partial_file_path(const fs::path& filename_stem) const {
        return get_config_directory_path() / (filename_stem.native() + ".partial");
    }

    std::string ServerContext::get_backup_file_path(const fs::path& filename_stem) const {
        return get_config_directory_path() / (filename_stem.native() + ".upload");
    }

    void ServerContext::set_path_of_client(ClientContext& client_context, std::filesystem::path new_path) {
        std::scoped_lock lock{m_path_locks_mutex};
        client_context.set_current_path(std::move(new_path));
    }

    void ServerContext::remove_expired_auth_tokens() {
        std::erase_if(m_auth_tokens, [](auto& ptr) { return ptr.expired(); });
    }

    void ServerContext::throw_on_invalid_path(const std::filesystem::path& path) const {
        if(!path.native().starts_with(get_root_path().native()) || !fs::exists(path)) {
            throw std::runtime_error{"ServerContext: invalid path to lock (not subpath of root)"};
        }
    }

    void ServerContext::throw_if_file_is_already_uploaded(const std::filesystem::path& path) const {
        std::shared_lock lock{m_path_locks_mutex};
        for(auto&& lock_ptr : std::as_const(m_path_locks)) {
            const std::shared_ptr<const PathLock> lock_shared_ptr = lock_ptr.lock();
            if(lock_shared_ptr != nullptr && lock_shared_ptr->get_locked_path() == path) {
                throw ProtocolError{ProtocolCode::in_use};
            }
        }
    }

    void ServerContext::insert_path_lock(std::weak_ptr<PathLock> path_lock) {
        std::scoped_lock lock{m_path_locks_mutex};
        remove_expired_path_locks();
        m_path_locks.emplace_back(std::move(path_lock));
    }

    void ServerContext::remove_expired_path_locks() {
        std::erase_if(m_path_locks, [](auto& ptr) { return ptr.expired(); });
    }
}
