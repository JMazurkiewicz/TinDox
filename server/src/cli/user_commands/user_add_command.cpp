#include "tds/cli/user_commands/user_add_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"

#include <iostream>

namespace tds::cli::user_commands {
    void UserAddCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() != 0) {
            throw InvalidCommandArgumentsError{"user add command does not take any arguments", "tds user add"};
        }
    }

    void UserAddCommand::execute() {
        std::cout << "user added!\n";
    }
}
