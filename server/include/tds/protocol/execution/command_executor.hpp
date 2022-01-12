#pragma once

#include "tds/command/command_executor.hpp"
#include "tds/command/no_such_command_error.hpp"
#include "tds/protocol/client_context.hpp"
#include "tds/protocol/execution/command.hpp"
#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/server_context.hpp"

#include <type_traits>

#include <fmt/core.h>

namespace tds::protocol::execution {
    template<Command... Commands>
    class CommandExecutor : public command::CommandExecutor<Commands...> {
    private:
        using Base = command::CommandExecutor<Commands...>;

    public:
        CommandExecutor(ServerContext& server_context, ClientContext& client_context)
            : m_server_context{server_context}
            , m_client_context{client_context} { }

        void set_command(std::string_view command_name) {
            try {
                Base::set_command(command_name);
                check_authorization();
                check_permissions();
                this->visit_command([&](auto& command) { command.set_server_context(m_server_context); });
                this->visit_command([&](auto& command) { command.set_client_context(m_client_context); });
                set_command_name();
            } catch(const command::NoSuchCommandError& e) {
                throw ProtocolError{ProtocolCode::bad_command, fmt::format("No such command '{}'", e.what())};
            } catch(const ProtocolError&) {
                throw;
            } catch(const std::exception& e) {
                throw ProtocolError{ProtocolCode::unknown, e.what()};
            }
        }

        void parse_fields(std::span<const Field> fields) {
            this->visit_command([&](auto& command) { command.parse_fields(fields); });
        }

        Response get_response() {
            return this->visit_command([&](auto& command) { return command.get_response(); });
        }

    private:
        void check_authorization() {
            this->visit_command([&]<typename T>(T&&) {
                using U = std::remove_cvref_t<T>;
                if(U::requires_authorization && !m_client_context.is_authorized()) {
                    throw ProtocolError{ProtocolCode::not_logged_in};
                }
            });
        }

        void check_permissions() {
            if(m_client_context.is_authorized()) {
                this->visit_command([&]<typename T>(T&&) {
                    using U = std::remove_cvref_t<T>;
                    if((U::required_perms & m_client_context.get_auth_token().get_perms()) != U::required_perms) {
                        throw ProtocolError{ProtocolCode::not_enough_perms};
                    }
                });
            }
        }

        void set_command_name() {
            this->visit_command([&]<typename T>(T&& command) {
                using U = std::remove_cvref_t<T>;
                command.set_command_name(U::name);
            });
        }

        ServerContext& m_server_context;
        ClientContext& m_client_context;
    };
}
