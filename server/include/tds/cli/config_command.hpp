#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class ConfigCommand : public CommandInterface<ConfigCommand> {
    public:
        static constexpr std::string_view name = "config";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();
    };
}
