#include "tds/cli/help_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view help_command::name() {
        return "help";
    }

    void help_command::execute(std::span<const std::string_view>) {
        std::cout << "Help info (work in progress).\n";
    }
}
