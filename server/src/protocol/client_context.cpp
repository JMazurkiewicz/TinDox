#include "tds/protocol/client_context.hpp"

#include "tds/protocol/protocol_error.hpp"

namespace fs = std::filesystem;

namespace tds::protocol {
    ClientContext::ClientContext()
        : m_alive{true}
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
        return m_token != nullptr;
    }

    void ClientContext::set_new_token(std::shared_ptr<AuthToken> token) {
        if(is_authorized()) {
            throw ProtocolError{ProtocolCode::user_already_logged};
        } else {
            m_token = std::move(token);
        }
    }

    const AuthToken& ClientContext::get_auth_token() const noexcept {
        return *m_token;
    }

    void ClientContext::logout() {
        m_token.reset();
    }

    const fs::path& ClientContext::get_current_path() {
        return m_current_path;
    }

    void ClientContext::set_current_path(fs::path path) {
        m_current_path = std::move(path);
    }
}
