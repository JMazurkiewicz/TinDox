#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class InitCommand : public CommandInterface<InitCommand> {
    public:
        static constexpr std::string_view name = "init";

        int do_execute(std::span<const std::string_view> args);
    };
}
