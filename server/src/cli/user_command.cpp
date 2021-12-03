#include "tds/cli/user_command.hpp"

#include <iostream>

namespace tds::cli {
    int UserCommand::do_execute(std::span<const std::string_view> args) {
        std::cout << "User\n";
        return 0;
    }
}
