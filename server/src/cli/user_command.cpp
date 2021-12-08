#include "tds/cli/user_command.hpp"

#include <iostream>

namespace tds::cli {
    void UserCommand::parse_arguments(std::span<const std::string_view>) { }

    void UserCommand::execute() {
        std::cout << "User\n";
    }
}
