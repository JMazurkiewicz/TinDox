#include "tds/cli/help_command.hpp"

#include <iostream>

namespace tds::cli {
    namespace {
        constexpr std::string_view help_message = {
            "usage: tds <command> [<args>]\n"
            "\n"
            "These are TinDoxServer commands:\n"
            "* config               modify server's properties\n"
            "* help                 display available options\n"
            "* init [<location>]    creates server instance in current or specified directory\n"
            "* log                  display logs\n"
            "* run                  run TinDoxServer\n"
            "* user                 add, modify or remove users\n"};
    }

    int HelpCommand::do_execute(std::span<const std::string_view> args) {
        std::cout << help_message;
        return !args.empty();
    }
}
