#pragma once

#include "tds/config/server_config.hpp"
#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/upload_token.hpp"
#include "tds/user/user_table.hpp"

#include <filesystem>
#include <mutex>
#include <shared_mutex>

namespace tds::protocol {
    class ServerContext {
    public:
        explicit ServerContext(std::filesystem::path root, const config::ServerConfig& server_config);
        ServerContext(const ServerContext&) = delete;
        ServerContext& operator=(const ServerContext&) = delete;

        [[nodiscard]] const std::filesystem::path& get_root_path() const noexcept;
        [[nodiscard]] const std::filesystem::path& get_config_directory_path() const noexcept;
        [[nodiscard]] std::filesystem::path get_relative_path_to(const std::filesystem::path& subpath) const;

        [[nodiscard]] std::shared_ptr<AuthToken> authorize_user(std::string_view username, const std::string& password);
        [[nodiscard]] std::shared_ptr<PathLock> lock_path(const std::filesystem::path& path);
        [[nodiscard]] std::shared_ptr<DownloadToken> download_file(const std::filesystem::path& path);
        [[nodiscard]] std::shared_ptr<UploadToken> upload_file(const std::filesystem::path& path, std::uintmax_t size);

        [[nodiscard]] bool is_path_forbidden(const std::filesystem::path& path) const;
        [[nodiscard]] bool is_path_locked(const std::filesystem::path& path) const;

        [[nodiscard]] std::string get_partial_file_path(const std::filesystem::path& filename_stem) const;
        [[nodiscard]] std::string get_backup_file_path(const std::filesystem::path& filename_stem) const;

    private:
        void remove_expired_auth_tokens();

        void throw_on_invalid_path(const std::filesystem::path& path) const;
        void insert_path_lock(std::weak_ptr<PathLock> path_lock);
        void remove_expired_path_locks();

        const std::filesystem::path m_root;
        const std::filesystem::path m_config_directory;
        const std::uintmax_t m_upload_max_size;

        std::mutex m_auth_mutex;
        user::UserTable m_user_table;
        std::vector<std::weak_ptr<AuthToken>> m_auth_tokens;

        mutable std::shared_mutex m_locks_mutex;
        std::vector<std::weak_ptr<PathLock>> m_path_locks;
    };
}
