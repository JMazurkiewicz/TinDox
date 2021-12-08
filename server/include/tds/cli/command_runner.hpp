#pragma once

#include "tds/cli/cli_error.hpp"
#include "tds/cli/command.hpp"

#include <variant>

namespace tds::cli {
    template<Command... Commands>
        requires(sizeof...(Commands) > 0)
    class CommandRunner {
    private:
        using CommandVariant = std::variant<std::monostate, Commands...>;
        static constexpr std::size_t variant_size_v = std::variant_size_v<CommandVariant>;

    public:
        CommandRunner() = default;
        CommandRunner(const CommandRunner&) = delete;
        CommandRunner& operator=(const CommandRunner&) = delete;

        int run(std::string_view command_name, std::span<const std::string_view> args) {
            construct_command(command_name);
            return execute_command(args);
        }

    private:
        template<std::size_t N = 1>
            requires(N < variant_size_v)
        void construct_command(std::string_view command_name) {
            if(get_command_name<N>() == command_name) {
                m_command.template emplace<N>();
            } else {
                construct_command<N + 1>(command_name);
            }
        }

        template<std::size_t N>
            requires(N == variant_size_v)
        [[noreturn]] void construct_command(std::string_view command_name) {
            throw CliError{"tds: '" + std::string{command_name} + "' is not a tds command. See 'tds help'."};
        }

        template<std::size_t N>
        static std::string_view get_command_name() {
            using Alternative = std::variant_alternative_t<N, CommandVariant>;
            return Alternative::get_name();
        }

        int execute_command(std::span<const std::string_view> args) {
            auto visitor = [args]<typename T>(T& command) {
                if constexpr(Command<T>) {
                    return command(args);
                } else {
                    return 1;
                }
            };

            return std::visit(visitor, m_command);
        }

        CommandVariant m_command;
    };
}
