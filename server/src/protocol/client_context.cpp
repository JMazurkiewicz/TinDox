#include "tds/protocol/client_context.hpp"

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
        return m_token.has_value();
    }

    void ClientContext::set_new_token(AuthToken new_token) {
        if(is_authorized()) {
            // TODO throw error??
        } else {
            m_token.emplace(std::move(new_token));
        }
    }

    const AuthToken& ClientContext::get_auth_token() const noexcept {
        return m_token.value();
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
