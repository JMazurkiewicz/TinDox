#pragma once

#include "tds/cli/cli_error.hpp"
#include "tds/cli/command.hpp"
#include "tds/cli/no_such_command_error.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <variant>

namespace tds::cli {
    template<Command... Commands>
        requires(sizeof...(Commands) > 0)
    class CommandExecutor {
    private:
        using Variant = std::variant<std::monostate, Commands...>;
        static constexpr std::size_t variant_size_v = std::variant_size_v<Variant>;

    public:
        CommandExecutor() = default;
        CommandExecutor(const CommandExecutor&) = delete;
        CommandExecutor& operator=(const CommandExecutor&) = delete;

        void set_command(std::string_view command_name) {
            construct_command(command_name);
        }

        template<typename F>
            requires(std::invocable<F, Commands&>&&...)
        decltype(auto) visit_command(F&& visitor) {
            using R = std::common_reference_t<std::invoke_result_t<F, Commands&>...>;

            auto first_visitor = [&]<typename T>(T& command) -> R {
                if constexpr(Command<T>) {
                    return std::invoke(std::forward<F>(visitor), command);
                } else {
                    throw std::runtime_error{"CommandExecutor: no command constructed"};
                }
            };

            return std::visit(first_visitor, m_command);
        }

        void parse_arguments(std::span<const std::string_view> args) {
            visit_command([args](auto& command) { command.parse_arguments(args); });
        }

        void execute() {
            visit_command([](auto& command) { command.execute(); });
        }

    private:
        template<std::size_t N = 1>
            requires(N < variant_size_v)
        void construct_command(std::string_view command_name) {
            constexpr std::string_view current_command_name = std::variant_alternative_t<N, Variant>::name;
            if(current_command_name == command_name) {
                m_command.template emplace<N>();
            } else {
                construct_command<N + 1>(command_name);
            }
        }

        template<std::size_t N>
            requires(N == variant_size_v)
        [[noreturn]] void construct_command(std::string_view command_name) {
            throw NoSuchCommandError{command_name};
        }

        Variant m_command;
    };
}
