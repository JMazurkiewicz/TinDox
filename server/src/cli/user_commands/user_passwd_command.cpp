#include "tds/cli/user_commands/user_passwd_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"

#include <iostream>

namespace tds::cli::user_commands {
    void UserPasswdCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() != 1) {
            throw InvalidCommandArgumentsError{"user passwd takes username as argument", "tds user passwd <username>"};
        }
    }

    void UserPasswdCommand::execute() {
        std::cout << "password changed!\n";
    }
}
