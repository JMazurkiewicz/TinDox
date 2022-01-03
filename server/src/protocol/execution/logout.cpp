#include "tds/protocol/execution/logout.hpp"

namespace tds::protocol::execution {
    void Logout::parse_fields(std::span<const Field> fields) { }

    void Logout::execute() {
        m_client_context->logout();
    }
}
