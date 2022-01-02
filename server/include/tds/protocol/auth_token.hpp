#pragma once

#include "tds/user/permissions.hpp"

#include <string>

namespace tds::protocol {
    class AuthToken {
    public:
        explicit AuthToken(std::string username, user::Permissions perms);
        AuthToken(const AuthToken&) = delete;
        AuthToken& operator=(const AuthToken&) = delete;

        std::string_view get_username() const noexcept;
        user::Permissions get_perms() const noexcept;

    private:
        std::string m_username;
        user::Permissions m_perms;
    };
}
