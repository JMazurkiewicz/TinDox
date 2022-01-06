#pragma once

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/path_lock.hpp"
#include "tds/user/user_table.hpp"

#include <filesystem>
#include <mutex>

namespace tds::protocol {
    class ServerContext {
    public:
        explicit ServerContext(std::filesystem::path root);
        ServerContext(const ServerContext&) = delete;
        ServerContext& operator=(const ServerContext&) = delete;

        [[nodiscard]] const std::filesystem::path& get_root_path() const noexcept;
        [[nodiscard]] const std::filesystem::path& get_config_directory_path() const noexcept;
        [[nodiscard]] std::filesystem::path get_relative_path_to(const std::filesystem::path& subpath) const;

        std::shared_ptr<AuthToken> authorize_user(std::string_view username, const std::string& password);
        std::shared_ptr<PathLock> lock_path(const std::filesystem::path& path);
        void register_download_token(std::weak_ptr<DownloadToken> download_token);

        bool is_path_forbidden(const std::filesystem::path& path) const;
        bool is_path_locked(const std::filesystem::path& path);

    private:
        bool has_user_logged_in(std::string_view username);

        void remove_dead_users();
        void remove_expired_path_locks();
        void remove_dead_download_tokens();

        bool is_locked_by_user(const std::filesystem::path& path);
        bool is_locked_by_download(const std::filesystem::path& path);

        const std::filesystem::path m_root;
        const std::filesystem::path m_config_directory;

        std::mutex m_auth_mutex;
        user::UserTable m_user_table;
        std::vector<std::weak_ptr<AuthToken>> m_auth_tokens;

        std::mutex m_locks_mutex;
        std::vector<std::weak_ptr<PathLock>> m_path_locks;
        std::vector<std::weak_ptr<DownloadToken>> m_download_tokens;
    };
}
