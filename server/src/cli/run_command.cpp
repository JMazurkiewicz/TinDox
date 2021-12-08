#include "tds/cli/run_command.hpp"

#include <iostream>

namespace tds::cli {
    void RunCommand::parse_arguments(std::span<const std::string_view>) { }

    void RunCommand::execute() {
        std::cout << "Run\n";
    }
}
