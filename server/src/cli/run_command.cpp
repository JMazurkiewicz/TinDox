#include "tds/cli/run_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view run_command::name() {
        return "run";
    }

    void run_command::execute(std::span<const std::string_view> args) {
        std::cout << "Run\n";
    }
}
