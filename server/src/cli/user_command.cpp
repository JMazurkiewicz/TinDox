#include "tds/cli/user_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view user_command::name() {
        return "user";
    }

    void user_command::execute(std::span<const std::string_view> args) {
        std::cout << "User\n";
    }
}
