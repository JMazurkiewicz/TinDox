#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    void CommandBase::set_server_context(const ServerContext& server_context) {
        m_server_context = &server_context;
    }

    void CommandBase::set_client_context(ClientContext& client_context) {
        m_client_context = &client_context;
    }

    Response CommandBase::get_response() {
        return m_response_builder.get_response();
    }
}
