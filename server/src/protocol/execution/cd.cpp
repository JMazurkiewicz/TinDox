#include "tds/protocol/execution/cd.hpp"

#include "tds/protocol/execution/path_based_command.hpp"

namespace tds::protocol::execution {
    void Cd::parse_fields(std::span<const Field> fields) {
        PathBasedCommand::parse_fields(fields);
        if(!has_path()) {
            throw ProtocolError{ProtocolCode::bad_field, "cd requires path field"};
        }
    }

    void Cd::execute() {
        m_client_context->set_current_path(std::move(get_path()));
        m_response_builder.add_line(
            m_server_context->get_relative_path_to(m_client_context->get_current_path()).native());
    }
}
