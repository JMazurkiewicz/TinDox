#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    CommandBase::CommandBase() {
        m_response_builder.set_code(ProtocolCode::ok);
    }

    void CommandBase::set_server_context(ServerContext& server_context) {
        m_server_context = &server_context;
    }

    void CommandBase::set_client_context(ClientContext& client_context) {
        m_client_context = &client_context;
    }

    void CommandBase::set_command_name(std::string_view command_name) {
        m_response_builder.set_command_name(command_name);
    }

    Response CommandBase::get_response() {
        return m_response_builder.get_response();
    }
}
