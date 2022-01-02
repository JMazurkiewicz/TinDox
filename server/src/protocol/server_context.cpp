#include "tds/protocol/server_context.hpp"
#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <algorithm>
#include <mutex>

namespace fs = std::filesystem;

namespace tds::protocol {
    ServerContext::ServerContext(fs::path root)
        : m_root{std::move(root)} {
        m_user_table.open(m_root / ".tds/users");
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

    AuthToken ServerContext::authorize_user(std::string_view username, const std::string& password) {
        std::lock_guard lock{m_mut};
        if(std::ranges::find(m_authorized_users, username) != m_authorized_users.end()) {
            throw ProtocolError{ProtocolCode::user_already_logged};
        }

        if(!m_user_table.has_user(username) || !m_user_table.verify_password_of_user(username, password)) {
            throw ProtocolError{ProtocolCode::invalid_credentials};
        }

        m_authorized_users.emplace_back(username);
        return AuthToken{std::string{username}, m_user_table.get_perms_of_user(username)};
    }

    void ServerContext::logout_user(std::string_view username) {
        std::lock_guard lock{m_mut};
        if(std::ranges::find(m_authorized_users, username) != m_authorized_users.end()) {
            std::erase(m_authorized_users, username);
        } else {
            throw ProtocolError{ProtocolCode::invalid_credentials};
        }
    }
}
