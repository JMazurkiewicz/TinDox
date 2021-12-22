#include "tds/user/user_table.hpp"

#include "tds/linux/hash.hpp"
#include "tds/user/user_table_error.hpp"

#include <algorithm>
#include <fstream>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace tds::user {
    bool UserTable::is_username_ok(std::string_view username) {
        const auto is_char_ok = [](unsigned char symbol) {
            return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z') ||
                   ('0' <= symbol && symbol <= '9') || symbol == '_';
        };
        return username.size() <= max_username_length && std::ranges::all_of(username, is_char_ok);
    }

    bool UserTable::is_password_ok(std::string_view password) {
        return password.size() <= max_password_length;
    }

    UserTable::UserTable(std::filesystem::path users_file)
        : m_users_file{std::move(users_file)} {
        fetch_table();
    }

    void UserTable::open(std::filesystem::path users_file) {
        m_users_file = std::move(users_file);
        fetch_table();
    }

    bool UserTable::has_user(std::string_view username) const {
        return std::ranges::find(m_users, username, &UserData::m_name) != m_users.end();
    }

    void UserTable::add_user(std::string_view username, const std::string& password) {
        if(!has_user(username)) {
            m_users.push_back({std::string{username}, linux::hash(password), Permissions::initial});
        }
    }

    void UserTable::remove_user(std::string_view username) {
        if(has_user(username)) {
            std::erase_if(m_users, [&](const UserData& user) { return user.m_name == username; });
        } else {
            throw UserTableError{fmt::format("user '{}' does not exist", username)};
        }
    }

    std::string_view UserTable::get_password_hash_of_user(std::string_view username) const {
        return get_user(username).m_password_hash;
    }

    bool UserTable::verify_password_of_user(std::string_view username, const std::string& password) const {
        return linux::hash(password) == get_user(username).m_password_hash;
    }

    void UserTable::set_password_of_user(std::string_view username, const std::string& new_password) {
        get_user(username).m_password_hash = linux::hash(new_password);
    }

    Permissions UserTable::get_perms_of_user(std::string_view username) const {
        return get_user(username).m_perms;
    }

    void UserTable::set_perms_of_user(std::string_view username, Permissions new_perms) {
        get_user(username).m_perms = new_perms;
    }

    void UserTable::commit_table() {
        if(std::ofstream out_file{m_users_file}; !out_file.good()) {
            throw UserTableError{fmt::format("failed to open output file ({})", m_users_file)};
        } else {
            out_file << create_toml_table();
        }
    }

    UserTable::UserData& UserTable::get_user(std::string_view username) {
        if(auto it = std::ranges::find(m_users, username, &UserData::m_name); it != m_users.end()) {
            return *it;
        } else {
            throw UserTableError{fmt::format("user '{}' does not exist", username)};
        }
    }

    const UserTable::UserData& UserTable::get_user(std::string_view username) const {
        if(auto it = std::ranges::find(m_users, username, &UserData::m_name); it != m_users.end()) {
            return *it;
        } else {
            throw UserTableError{fmt::format("user '{}' does not exist", username)};
        }
    }

    void UserTable::fetch_table() {
        const auto table = toml::parse_file(m_users_file.native());
        const auto users = table["users"];
        if(!(users && users.is_array())) {
            throw UserTableError{"TOML object 'users' is invalid"};
        }

        for(auto&& user : *users.as_array()) {
            if(auto user_data_ptr = user.as_table()) {
                fetch_one_user(*user_data_ptr);
            } else {
                throw UserTableError{"invalid user"};
            }
        }
    }

    void UserTable::fetch_one_user(const toml::table& table) {
        for(auto required_field : {"name", "passwd", "perms"}) {
            if(!(table.contains(required_field) && table[required_field].is_string())) {
                throw UserTableError{fmt::format("invalid required filed '{}'", required_field)};
            }
        }

        m_users.push_back({.m_name = table["name"].as_string()->get(),
                           .m_password_hash = table["passwd"].as_string()->get(),
                           .m_perms = perms_from_string(table["perms"].as_string()->get())});
    }

    toml::table UserTable::create_toml_table() {
        toml::array user_array;
        for(auto&& user : m_users) {
            toml::table user_table;
            user_table.insert("name", user.m_name);
            user_table.insert("passwd", user.m_password_hash);
            user_table.insert("perms", to_string(user.m_perms));
            user_array.push_back(std::move(user_table));
        }

        toml::table main_table;
        main_table.insert("users", std::move(user_array));
        return main_table;
    }
}
