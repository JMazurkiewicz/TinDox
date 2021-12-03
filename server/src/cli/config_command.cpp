#include "tds/cli/config_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view config_command::name() {
        return "config";
    }

    void config_command::execute(std::span<const std::string_view>) {
        std::cout << "Config\n";
    }
}
