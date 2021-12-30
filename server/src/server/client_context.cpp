#include "tds/server/client_context.hpp"

namespace tds::server {
    ClientContext::ClientContext()
        : m_alive{true} { }

    bool ClientContext::is_alive() const noexcept {
        return m_alive;
    }

    void ClientContext::kill() {
        m_alive = false;
    }
}
