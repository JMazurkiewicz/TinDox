#include "tds/cli/init_command.hpp"

#include <iostream>

namespace tds::cli {
    int InitCommand::do_execute(std::span<const std::string_view> args) {
        std::cout << "Init\n";
        return 0;
    }
}
