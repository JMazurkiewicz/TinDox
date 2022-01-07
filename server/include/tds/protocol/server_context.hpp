#pragma once

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/path_lock.hpp"
#include "tds/protocol/upload_token.hpp"
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

        [[nodiscard]] std::shared_ptr<AuthToken> authorize_user(std::string_view username, const std::string& password);
        [[nodiscard]] std::shared_ptr<PathLock> lock_path(const std::filesystem::path& path);
        [[nodiscard]] std::shared_ptr<DownloadToken> download_file(const std::filesystem::path& path);
        [[nodiscard]] std::shared_ptr<UploadToken> upload_file(const std::filesystem::path& path, std::uintmax_t size);

        [[nodiscard]] bool is_path_forbidden(const std::filesystem::path& path) const;
        [[nodiscard]] bool is_path_locked(const std::filesystem::path& path);

        [[nodiscard]] std::string get_partial_file_path(const std::filesystem::path& filename_stem) const;
        [[nodiscard]] std::string get_backup_file_path(const std::filesystem::path& filename_stem) const;

    private:
        bool is_user_authorized(std::string_view username);
        void check_path(const std::filesystem::path& path);

        void remove_expired_auth_tokens();
        void remove_expired_path_locks();

        const std::filesystem::path m_root;
        const std::filesystem::path m_config_directory;

        std::mutex m_auth_mutex;
        user::UserTable m_user_table;
        std::vector<std::weak_ptr<AuthToken>> m_auth_tokens;

        std::mutex m_locks_mutex;
        std::vector<std::weak_ptr<PathLock>> m_path_locks;
    };
}
