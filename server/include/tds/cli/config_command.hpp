#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class ConfigCommand : public CommandInterface<ConfigCommand> {
    public:
        static constexpr std::string_view name = "config";

        int do_execute([[maybe_unused]] std::span<const std::string_view>);
    };

    static_assert(Command<ConfigCommand>);
}
