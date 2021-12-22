#include "tds/cli/user_commands/user_remove_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"

#include <fmt/core.h>

namespace tds::cli::user_commands {
    void UserRemoveCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() != 1) {
            throw InvalidCommandArgumentsError{"user remove takes username as argument", "tds user remove <username>"};
        } else {
            m_username = args[0];
        }
    }

    void UserRemoveCommand::execute() {
        get_user_table().remove_user(m_username);
    }
}
