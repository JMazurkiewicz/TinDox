#include "tds/cli/help_command.hpp"

#include <iostream>

namespace tds::cli {
    namespace {
        constexpr std::string_view help_message = {
            "usage: tds <command> [<args>]\n"
            "\n"
            "These are TinDoxServer commands:\n"
            "* config       modify server's properties\n"
            "* help         display available options\n"
            "* init         creates server instance in current or selected directory\n"
            "* log          display logs\n"
            "* run          run TinDoxServer\n"
            "* user         add, modify or remove users\n"};
    }

    std::string_view help_command::name() {
        return "help";
    }

    void help_command::execute(std::span<const std::string_view>) {
        std::cout << help_message;
    }
}
