#include "tds/cli/user_commands/user_perms_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"

#include <iostream>

namespace tds::cli::user_commands {
    void UserPermsCommand::parse_arguments(std::span<const std::string_view> args) {
        /// @todo
    }

    void UserPermsCommand::execute() {
        std::cout << "perms changed!\n";
    }
}
