#pragma once

#include "tds/cli/command_interface.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    class HelpCommand : public CommandInterface<HelpCommand> {
    public:
        static constexpr std::string_view name = "help";

        HelpCommand();
        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void print_help();

        bool m_empty_args;
    };
}
