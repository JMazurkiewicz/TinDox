#pragma once

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/protocol_mode.hpp"
#include "tds/protocol/upload_token.hpp"
#include "tds/user/permissions.hpp"

#include <filesystem>
#include <memory>

namespace tds::protocol {
    class ClientContext {
    public:
        explicit ClientContext();
        ClientContext(const ClientContext&) = delete;
        ClientContext& operator=(const ClientContext&) = delete;

        bool is_alive() const noexcept;
        void kill();

        ProtocolMode get_mode() const noexcept;
        void set_mode(ProtocolMode mode);

        bool is_authorized() const noexcept;
        void set_auth_token(std::shared_ptr<AuthToken> token);
        int get_auth_try_count() const noexcept;
        const AuthToken& get_auth_token() const noexcept;
        void logout();

        void set_path_lock(std::shared_ptr<PathLock> lock);
        const std::filesystem::path& get_current_path();
        void set_current_path(std::filesystem::path path);

        bool has_download_token() const noexcept;
        void set_download_token(std::shared_ptr<DownloadToken> token);
        void set_downloaded_file_offset(std::uintmax_t offset);
        [[nodiscard]] std::shared_ptr<DownloadToken> get_download_token();

        void set_upload_token(std::shared_ptr<UploadToken> token);
        [[nodiscard]] std::shared_ptr<UploadToken> get_upload_token();

    private:
        bool m_alive : 1;
        int m_auth_try_count : 7;
        ProtocolMode m_mode;
        std::shared_ptr<AuthToken> m_auth_token;
        std::shared_ptr<PathLock> m_current_path_lock;
        std::shared_ptr<DownloadToken> m_download_token;
        std::shared_ptr<UploadToken> m_upload_token;
    };
}
