#include "tds/protocol/auth_token.hpp"

namespace tds::protocol {
    AuthToken::AuthToken(std::string username, user::Permissions perms)
        : m_username{std::move(username)}
        , m_perms{perms} { }

    std::string_view AuthToken::get_username() const noexcept {
        return m_username;
    }

    user::Permissions AuthToken::get_perms() const noexcept {
        return m_perms;
    }
}
