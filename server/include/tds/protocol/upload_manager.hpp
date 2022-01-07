#pragma once

#include "tds/protocol/server_context.hpp"
#include "tds/protocol/upload_token.hpp"

#include <fstream>
#include <span>

namespace tds::protocol {
    class UploadManager {
    public:
        explicit UploadManager(const ServerContext& server_context);
        UploadManager(const UploadManager&) = delete;
        UploadManager& operator=(const UploadManager&) = delete;
        ~UploadManager();

        void start_upload(std::shared_ptr<UploadToken> token);
        std::span<const char> commit_bytes(std::span<const char> bytes);
        bool has_finished() const noexcept;

    private:
        std::ios_base::openmode get_openmode() const noexcept;
        std::size_t get_remaining_bytes() const noexcept;
        void finish();
        void create_backup();

        const ServerContext& m_server_context;
        std::shared_ptr<UploadToken> m_token;
        std::filesystem::path m_partial_file_name;
        std::filesystem::path m_backup_file_name;
        std::ofstream m_file;
    };
}
