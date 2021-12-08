#include "tds/cli/log_command.hpp"

#include <iostream>

namespace tds::cli {
    void LogCommand::parse_arguments(std::span<const std::string_view>) { }

    void LogCommand::execute() {
        std::cout << "Log\n";
    }
}
