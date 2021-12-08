#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class UserCommand : public CommandInterface<UserCommand> {
    public:
        static constexpr std::string_view name = "user";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();
    };
}
