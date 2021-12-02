#include "tds/cli/init_command.hpp"

#include <iostream>

namespace tds::cli {
    std::string_view init_command::name() {
        return "init";
    }

    void init_command::execute(std::span<const std::string_view> args) {
        std::cout << "Init\n";
    }
}
