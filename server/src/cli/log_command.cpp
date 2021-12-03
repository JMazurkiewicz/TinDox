#include "tds/cli/log_command.hpp"

#include <iostream>

namespace tds::cli {
    int LogCommand::do_execute(std::span<const std::string_view> args) {
        std::cout << "Log\n";
        return 0;
    }
}
