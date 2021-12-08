#include "tds/cli/help_command.hpp"

#include <iostream>

namespace tds::cli {
    HelpCommand::HelpCommand()
        : m_empty_args{true} { }

    void HelpCommand::parse_arguments(std::span<const std::string_view> args) {
        m_empty_args = args.empty();
    }

    void HelpCommand::execute() {
        print_help();

        if(!m_empty_args) {
            set_error();
        }
    }

    void HelpCommand::print_help() {
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
        std::cout << help_message;
    }
}
