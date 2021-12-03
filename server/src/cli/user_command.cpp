#include "tds/cli/user_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view UserCommand::name() {
        return "user";
    }

    void UserCommand::execute(std::span<const std::string_view> args) {
        std::cout << "User\n";
    }
}
