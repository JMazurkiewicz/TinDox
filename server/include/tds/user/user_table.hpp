#pragma once

#include "tds/user/permissions.hpp"

#include <filesystem>
#include <string_view>
#include <vector>

#include <toml++/toml.h>

namespace tds::user {
    class UserTable {
    public:
        static constexpr std::size_t max_username_length = 40;
        static constexpr std::size_t max_password_length = 40;

        static bool is_username_ok(std::string_view username);
        static bool is_password_ok(std::string_view password);

        UserTable() = default;
        UserTable(std::filesystem::path users_file);

        UserTable(const UserTable&) = delete;
        UserTable& operator=(const UserTable&) = delete;

        void open(std::filesystem::path users_file);
        bool has_user(std::string_view username) const;

        void add_user(std::string_view username, const std::string& password);
        void remove_user(std::string_view username);

        std::string_view get_password_hash_of_user(std::string_view username);
        bool verify_password_of_user(std::string_view username, const std::string& password) const;
        void set_password_of_user(std::string_view username, const std::string& new_password);

        Permissions get_perms_of_user(std::string_view username);
        void set_perms_of_user(std::string_view username, Permissions new_perms);

        void commit_table();

    private:
        struct UserData {
            std::string m_name;
            std::string m_password_hash;
            Permissions m_perms;
        };

        UserData& get_user(std::string_view username);
        const UserData& get_user(std::string_view username) const;

        void fetch_table();
        void fetch_one_user(const toml::table& table);
        toml::table create_toml_table();

        std::filesystem::path m_users_file;
        std::vector<UserData> m_users;
    };
}
