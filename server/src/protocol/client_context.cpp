#include "tds/protocol/client_context.hpp"

#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

namespace fs = std::filesystem;

namespace tds::protocol {
    ClientContext::ClientContext()
        : m_alive{true}
        , m_auth_try_count{0}
        , m_mode{ProtocolMode::command} { }

    bool ClientContext::is_alive() const noexcept {
        return m_alive;
    }

    void ClientContext::kill() {
        m_alive = false;
    }

    ProtocolMode ClientContext::get_mode() const noexcept {
        return m_mode;
    }

    void ClientContext::set_mode(ProtocolMode mode) {
        m_mode = mode;
    }

    bool ClientContext::is_authorized() const noexcept {
        return m_auth_token != nullptr;
    }

    void ClientContext::set_auth_token(std::shared_ptr<AuthToken> token) {
        if(token == nullptr) {
            ++m_auth_try_count;
        } else if(is_authorized()) {
            throw ProtocolError{ProtocolCode::user_already_logged};
        } else {
            m_auth_token = std::move(token);
            m_auth_try_count = 0;
        }
    }

    int ClientContext::get_auth_try_count() const noexcept {
        return m_auth_try_count;
    }

    const AuthToken& ClientContext::get_auth_token() const noexcept {
        return *m_auth_token;
    }

    void ClientContext::logout() {
        m_auth_token.reset();
    }

    void ClientContext::set_path_lock(std::shared_ptr<PathLock> lock) {
        m_current_path_lock = lock;
    }

    const fs::path& ClientContext::get_current_path() {
        return m_current_path_lock->get_locked_path();
    }

    void ClientContext::set_current_path(fs::path path) {
        m_current_path_lock->set_locked_path(std::move(path));
    }

    bool ClientContext::has_download_token() const noexcept {
        return m_download_token != nullptr;
    }

    void ClientContext::set_download_token(std::shared_ptr<DownloadToken> token) {
        m_download_token = std::move(token);
    }

    void ClientContext::set_downloaded_file_offset(std::uintmax_t offset) {
        m_download_token->set_file_offset(offset);
    }

    std::shared_ptr<DownloadToken> ClientContext::get_download_token() {
        return std::move(m_download_token);
    }

    void ClientContext::set_upload_token(std::shared_ptr<UploadToken> token) {
        m_upload_token = std::move(token);
    }

    std::shared_ptr<UploadToken> ClientContext::get_upload_token() {
        return std::move(m_upload_token);
    }
}
