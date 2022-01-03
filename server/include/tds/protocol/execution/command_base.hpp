#pragma once

#include "tds/protocol/client_context.hpp"
#include "tds/protocol/response_builder.hpp"
#include "tds/protocol/server_context.hpp"

namespace tds::protocol::execution {
    class CommandBase {
    public:
        CommandBase();
        CommandBase(const CommandBase&) = delete;
        CommandBase& operator=(const CommandBase&) = delete;

        void set_server_context(ServerContext& server_context);
        void set_client_context(ClientContext& client_context);
        void set_command_name(std::string_view command_name);

        Response get_response();

    protected:
        ServerContext* m_server_context;
        ClientContext* m_client_context;

        ResponseBuilder m_response_builder;
    };
}
