#include "tds/cli/log_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view log_command::name() {
        return "log";
    }

    void log_command::execute(std::span<const std::string_view> args) {
        std::cout << "Log\n";
    }
}
