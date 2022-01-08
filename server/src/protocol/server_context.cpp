#include "tds/protocol/server_context.hpp"

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/path_lock.hpp"
#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <algorithm>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

namespace tds::protocol {
    ServerContext::ServerContext(fs::path root)
        : m_root{std::move(root)}
        , m_config_directory{m_root / ".tds"} {
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
        std::lock_guard lock{m_auth_mutex};
        remove_expired_auth_tokens();

        if(!m_user_table.has_user(username) || !m_user_table.verify_password_of_user(username, password)) {
            throw ProtocolError{ProtocolCode::invalid_credentials};
        } else if(is_user_authorized(username)) {
            throw ProtocolError{ProtocolCode::user_already_logged};
        }

        auto auth_token = std::make_shared<AuthToken>(std::string{username}, m_user_table.get_perms_of_user(username));
        m_auth_tokens.emplace_back(auth_token);
        return auth_token;
    }

    std::shared_ptr<PathLock> ServerContext::lock_path(const std::filesystem::path& path) {
        std::lock_guard lock{m_locks_mutex};
        remove_expired_path_locks();
        check_path(path);

        auto path_lock = make_path_lock(path);
        m_path_locks.emplace_back(path_lock);
        return path_lock;
    }

    std::shared_ptr<DownloadToken> ServerContext::download_file(const std::filesystem::path& path) {
        std::lock_guard lock{m_locks_mutex};
        remove_expired_path_locks();
        check_path(path);

        auto download_lock = make_download_token(path);
        m_path_locks.emplace_back(download_lock);
        return download_lock;
    }

    std::shared_ptr<UploadToken> ServerContext::upload_file(const std::filesystem::path& path, std::uintmax_t size) {
        std::lock_guard loc{m_locks_mutex};
        remove_expired_path_locks();

        if(!path.is_absolute()) {
            throw ProtocolError{ProtocolCode::invalid_file_type};
        } else if(fs::exists(path)) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        } else if(!fs::exists(path.parent_path())) {
            throw ProtocolError{ProtocolCode::not_found};
        }

        for(auto&& lock : m_path_locks) {
            if(lock.lock()->get_locked_path() == path) {
                throw ProtocolError{ProtocolCode::in_use};
            }
        }

        auto upload_lock = make_upload_token(path, size);
        m_path_locks.emplace_back(upload_lock);
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

    bool ServerContext::is_path_locked(const std::filesystem::path& path) {
        std::lock_guard lock{m_locks_mutex};
        remove_expired_path_locks();

        return std::ranges::find_if(m_path_locks, [&](auto& ptr) { return ptr.lock()->has_locked_path(path); }) !=
               m_path_locks.end();
    }

    std::string ServerContext::get_partial_file_path(const fs::path& filename_stem) const {
        return get_config_directory_path() / (filename_stem.native() + ".partial");
    }

    std::string ServerContext::get_backup_file_path(const fs::path& filename_stem) const {
        return get_config_directory_path() / (filename_stem.native() + ".upload");
    }

    bool ServerContext::is_user_authorized(std::string_view username) {
        return std::ranges::find(m_auth_tokens, username, [](auto& ptr) { return ptr.lock()->get_username(); }) !=
               m_auth_tokens.end();
    }

    void ServerContext::check_path(const std::filesystem::path& path) {
        if(!path.native().starts_with(get_root_path().native()) || !fs::exists(path)) {
            throw std::runtime_error{"ServerContext: invalid path to lock (not subpath of root)"};
        }
    }

    void ServerContext::remove_expired_auth_tokens() {
        std::erase_if(m_auth_tokens, [](auto& ptr) { return ptr.expired(); });
    }

    void ServerContext::remove_expired_path_locks() {
        std::erase_if(m_path_locks, [](auto& ptr) { return ptr.expired(); });
    }
}
