#include "tds/cli/log_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view LogCommand::name() {
        return "log";
    }

    void LogCommand::execute(std::span<const std::string_view> args) {
        std::cout << "Log\n";
    }
}
