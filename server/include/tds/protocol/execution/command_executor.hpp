#pragma once

#include "tds/command/command_executor.hpp"
#include "tds/command/no_such_command_error.hpp"
#include "tds/protocol/client_context.hpp"
#include "tds/protocol/execution/command.hpp"
#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/server_context.hpp"

#include <fmt/core.h>

namespace tds::protocol::execution {
    template<Command... Commands>
    class CommandExecutor : public command::CommandExecutor<Commands...> {
    public:
        using Base = command::CommandExecutor<Commands...>;

        CommandExecutor(const ServerContext& server_context, ClientContext& client_context)
            : m_server_context{server_context}
            , m_client_context{client_context} { }

        void set_command(std::string_view command_name) {
            try {
                Base::set_command(command_name);
                this->visit_command([&](auto& command) { command.set_server_context(m_server_context); });
                this->visit_command([&](auto& command) { command.set_client_context(m_client_context); });
            } catch(const command::NoSuchCommandError& e) {
                throw ProtocolError{ProtocolCode::bad_command, fmt::format("No such command '{}'", e.what())};
            }
        }

        void parse_fields(std::span<const Field> fields) {
            this->visit_command([&](auto& command) { command.parse_fields(fields); });
        }

        Response get_response() {
            return this->visit_command([&](auto& command) { return command.get_response(); });
        }

    private:
        const ServerContext& m_server_context;
        ClientContext& m_client_context;
    };
}
