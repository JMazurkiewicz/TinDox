#pragma once

#include "tds/user/permissions.hpp"

#include <filesystem>

namespace tds::protocol {
    class AuthToken {
    public:
        explicit AuthToken(std::string username, user::Permissions perms);
        AuthToken(const AuthToken&) = delete;
        AuthToken& operator=(const AuthToken&) = delete;

        std::string_view get_username() const noexcept;
        user::Permissions get_perms() const noexcept;

        const std::filesystem::path& get_current_path() const noexcept;
        void set_current_path(std::filesystem::path path);

    private:
        std::string m_username;
        user::Permissions m_perms;
        std::filesystem::path m_current_path;
    };
}
