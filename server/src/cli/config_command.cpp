#include "tds/cli/config_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view ConfigCommand::name() {
        return "config";
    }

    void ConfigCommand::execute(std::span<const std::string_view>) {
        std::cout << "Config\n";
    }
}
