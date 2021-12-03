#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class HelpCommand : public CommandInterface<HelpCommand> {
    public:
        static constexpr std::string_view name = "help";

        int do_execute(std::span<const std::string_view> args);
    };
}
