#include "tds/protocol/execution/pwd.hpp"

namespace tds::protocol::execution {
    void Pwd::parse_fields(std::span<const Field> fields) { }

    void Pwd::execute() {
        m_response_builder.set_code(ProtocolCode::ok);
        m_response_builder.set_command_name("pwd");
        m_response_builder.add_line(
            m_server_context->get_relative_path_of(m_client_context->get_current_path()).native());
    }
}
