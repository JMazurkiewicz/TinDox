#pragma once

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/download_token.hpp"
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

        std::shared_ptr<AuthToken> authorize_user(std::string_view username, const std::string& password);
        void register_download_token(std::weak_ptr<DownloadToken> download_token);

    private:
        void remove_dead_users();
        bool has_user_logged_in(std::string_view username);

        void remove_dead_download_tokens();

        std::mutex m_mut;
        const std::filesystem::path m_root;
        user::UserTable m_user_table;

        std::vector<std::weak_ptr<AuthToken>> m_auth_tokens;
        std::vector<std::weak_ptr<DownloadToken>> m_download_tokens;
    };
}
