#include "tds/cli/config_command.hpp"

#include <iostream>

namespace tds::cli {
    void ConfigCommand::parse_arguments(std::span<const std::string_view>) { }

    void ConfigCommand::execute() {
        std::cout << "Config\n";
    }
}
