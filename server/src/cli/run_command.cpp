#include "tds/cli/run_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view RunCommand::name() {
        return "run";
    }

    void RunCommand::execute(std::span<const std::string_view> args) {
        std::cout << "Run\n";
    }
}
