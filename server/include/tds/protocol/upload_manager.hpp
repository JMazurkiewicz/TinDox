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
        void append(std::span<const char> bytes);
        bool has_finished();

    private:
        std::ios_base::openmode get_openmode() const noexcept;

        const std::filesystem::path m_temporary_path;
        std::shared_ptr<UploadToken> m_token;
        std::ofstream m_file;
    };
}
