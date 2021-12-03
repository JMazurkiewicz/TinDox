#include "tds/cli/init_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view InitCommand::name() {
        return "init";
    }

    void InitCommand::execute(std::span<const std::string_view> args) {
        std::cout << "Init\n";
    }
}
