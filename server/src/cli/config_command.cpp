#include "tds/cli/config_command.hpp"

#include <iostream>

namespace tds::cli {
    int ConfigCommand::do_execute(std::span<const std::string_view>) {
        std::cout << "Config\n";
        return 0;
    }
}
