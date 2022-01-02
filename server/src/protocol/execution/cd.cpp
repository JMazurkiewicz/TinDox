#include "tds/protocol/execution/cd.hpp"

namespace tds::protocol::execution {
    void Cd::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "path") {
                m_next_path = m_client_context->get_current_path() / field.get_string().value(); // TODO use operator*
            }
        }
    }

    void Cd::execute() {
        // TODO check if path is fine (CORRECT AND NOT BELOW ROOT)
        m_response_builder.set_code(ProtocolCode::ok);
        m_response_builder.set_command_name("cd");

        m_client_context->set_current_path(std::move(m_next_path));
        m_response_builder.add_line(
            '"' + m_server_context->get_relative_path_of(m_client_context->get_current_path()).native() + '"');
    }
}
