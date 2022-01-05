#include "tds/protocol/server_context.hpp"

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <algorithm>
#include <mutex>

namespace fs = std::filesystem;

namespace tds::protocol {
    ServerContext::ServerContext(fs::path root)
        : m_root{std::move(root)}
        , m_config_directory{m_root / ".tds"} {
        m_user_table.open(m_config_directory / "users");
    }

    const fs::path& ServerContext::get_root_path() const noexcept {
        return m_root;
    }

    fs::path ServerContext::get_relative_path_of(const fs::path& subpath) const {
        fs::path result = fs::relative(subpath, get_root_path());
        if(result == ".") {
            result.clear();
        }
        return fs::path{"/"} / std::move(result);
    }

    std::shared_ptr<AuthToken> ServerContext::authorize_user(std::string_view username, const std::string& password) {
        std::lock_guard lock{m_mut};
        remove_dead_users();
        if(has_user_logged_in(username)) {
            throw ProtocolError{ProtocolCode::user_already_logged};
        }

        if(!m_user_table.has_user(username) || !m_user_table.verify_password_of_user(username, password)) {
            throw ProtocolError{ProtocolCode::invalid_credentials};
        }

        auto new_token = std::make_shared<AuthToken>(std::string{username}, m_user_table.get_perms_of_user(username));
        m_auth_tokens.emplace_back(new_token);
        return new_token;
    }

    void ServerContext::register_download_token(std::weak_ptr<DownloadToken> download_token) {
        std::lock_guard lock{m_mut};
        remove_dead_download_tokens();

        if(!fs::exists(download_token.lock()->get_file_path())) {
            throw ProtocolError{ProtocolCode::not_found};
        } else {
            m_download_tokens.emplace_back(std::move(download_token));
        }
    }

    bool ServerContext::is_forbidden(const std::filesystem::path& path) const {
        if(!path.is_absolute()) {
            throw ProtocolError{ProtocolCode::unknown,
                                "Server context requires absolute paths. Please send this to server developer."};
        } else {
            return path.native().starts_with(m_config_directory.native());
        }
    }

    bool ServerContext::is_locked(const std::filesystem::path& path) {
        std::lock_guard lock{m_mut};
        return is_locked_by_user(path) || is_locked_by_download(path);
    }

    bool ServerContext::has_user_logged_in(std::string_view username) {
        return std::ranges::find(m_auth_tokens, username, [](auto& ptr) { return ptr.lock()->get_username(); }) !=
               m_auth_tokens.end();
    }

    void ServerContext::remove_dead_users() {
        std::erase_if(m_auth_tokens, [](auto& ptr) { return ptr.expired(); });
    }

    void ServerContext::remove_dead_download_tokens() {
        std::erase_if(m_download_tokens, [](auto& ptr) { return ptr.expired(); });
    }

    bool ServerContext::is_locked_by_user(const fs::path& path) {
        remove_dead_users();
        return std::ranges::find_if(m_auth_tokens, [&](auto& ptr) {
                   return ptr.lock()->get_current_path().native().starts_with(path.native());
               }) != m_auth_tokens.end();
    }

    bool ServerContext::is_locked_by_download(const fs::path& path) {
        remove_dead_download_tokens();
        return std::ranges::find(m_download_tokens, path, [](auto& ptr) { return ptr.lock()->get_file_path(); }) !=
               m_download_tokens.end();
    }
}
