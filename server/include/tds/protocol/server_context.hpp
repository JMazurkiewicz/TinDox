#pragma once

#include "tds/protocol/auth_token.hpp"
#include "tds/user/user_table.hpp"

#include <filesystem>
#include <mutex>

namespace tds::protocol {
    class ServerContext {
    public:
        explicit ServerContext(std::filesystem::path root);
        ServerContext(const ServerContext&) = delete;
        ServerContext& operator=(const ServerContext&) = delete;

        const std::filesystem::path& get_root_path() const noexcept;
        [[nodiscard]] std::filesystem::path get_relative_path_of(const std::filesystem::path& subpath) const;

        AuthToken authorize_user(std::string_view username, const std::string& password);
        void logout_user(std::string_view username);

    private:
        const std::filesystem::path m_root;

        std::mutex m_mut;
        user::UserTable m_user_table;
        std::vector<std::string> m_authorized_users;
    };
}
