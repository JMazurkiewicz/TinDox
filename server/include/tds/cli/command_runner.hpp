#pragma once

#include "tds/cli/cli_error.hpp"
#include "tds/cli/command.hpp"

#include <variant>

namespace tds::cli {
    template<command... Commands>
        requires(sizeof...(Commands) > 0)
    class command_runner {
    private:
        using command_variant = std::variant<Commands...>;

    public:
        command_runner() = default;
        command_runner(const command_runner&) = delete;
        command_runner& operator=(const command_runner&) = delete;

        void run(std::string_view command_name, std::span<const std::string_view> args) {
            construct_command(command_name);
            execute_command(args);
        }

    private:
        template<std::size_t N = 0>
            requires(N < sizeof...(Commands))
        void construct_command(std::string_view command_name) {
            if(get_command_name<N>() == command_name) {
                m_commands.template emplace<N>();
            } else {
                construct_command<N + 1>(command_name);
            }
        }

        template<std::size_t N>
            requires(N == sizeof...(Commands))
        [[noreturn]] void construct_command(std::string_view command_name) {
            throw cli_error("tds: '" + std::string{command_name} + "' is not a tds command. See 'tds help'.");
        }

        template<std::size_t N>
        static std::string_view get_command_name() {
            using alternative = std::variant_alternative_t<N, command_variant>;
            return alternative::name();
        }

        void execute_command(std::span<const std::string_view> args) {
            auto visitor = [args](auto& command) {
                command.execute(args);
            };

            std::visit(visitor, m_commands);
        }

        command_variant m_commands;
    };
}
