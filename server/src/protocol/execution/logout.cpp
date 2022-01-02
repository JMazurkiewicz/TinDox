#include "tds/protocol/execution/logout.hpp"

namespace tds::protocol::execution {
    void Logout::parse_fields(std::span<const Field> fields) { }

    void Logout::execute() {
        m_response_builder.set_code(ProtocolCode::ok);
        m_response_builder.set_command_name(name);
        m_client_context->logout();
    }
}
