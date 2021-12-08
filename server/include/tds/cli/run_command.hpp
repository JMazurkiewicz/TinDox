#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class RunCommand : public CommandInterface<RunCommand> {
    public:
        static constexpr std::string_view name = "run";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();
    };
}
