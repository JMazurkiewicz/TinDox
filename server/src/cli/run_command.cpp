#include "tds/cli/run_command.hpp"

#include <iostream>

namespace tds::cli {
    int RunCommand::do_execute(std::span<const std::string_view> args) {
        std::cout << "Run\n";
        return 0;
    }
}
