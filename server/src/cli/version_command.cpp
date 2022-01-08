#include "tds/cli/version_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/config.hpp"

#include <iostream>

namespace tds::cli {
    void VersionCommand::parse_arguments(std::span<const std::string_view> args) {
        m_empty_args = args.empty();
    }

    void VersionCommand::execute() {
        std::cout << "TinDox Server " TDS_VERSION_STRING "\n";
        if(!m_empty_args) {
            throw InvalidCommandArgumentsError{"help command takes no arguments", "tds help"};
        }
    }
}
