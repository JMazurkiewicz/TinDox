#pragma once

#include "tds/cli/command.hpp"
#include "tds/command/command_executor.hpp"

namespace tds::cli {
    template<Command... Commands>
        requires(sizeof...(Commands) > 0)
    class CommandExecutor : public command::CommandExecutor<Commands...> {
    public:
        void parse_arguments(std::span<const std::string_view> args) {
            this->visit_command([args](auto& command) { command.parse_arguments(args); });
        }
    };
}
