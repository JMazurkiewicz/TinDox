#include "tds/cli/help_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"

#include <iostream>

namespace tds::cli {
    namespace {
        inline constexpr std::string_view help_message = {
            "usage: tds <command> [args]\n"
            "\n"
            "These are TinDoxServer commands:\n"
            "* help                                                      display available options\n"
            "* init [director]                                           creates server instance in current or "
            "specified directory\n"
            "* run [<-d|--dir> directory] [<-p|--port> port] [--debug]   run TinDox server\n"
            "* user [<-d|--dir> <tds-dir>] <add|remove|passwd|perms>     add, modify or remove users\n"
            "\n"
            "User commands:\n"
            "* user add                                                  adds new user\n"
            "* user remove <username>                                    remove specified user\n"
            "* user passwd <username>                                    change user's password\n"
            "* user perms <username> [+perms] [-perms]                   grant or remove permissions of user\n"
            "\n"
            "Available permissions:\n"
            "* write    [w] - commands like rm, rename, mkdir\n"
            "* copy     [c] - cp command\n"
            "* move     [m] - mv command\n"
            "* download [d] - dl and dls commands\n"
            "* upload   [u] - ul and uls commands\n"};
    }

    void HelpCommand::parse_arguments(std::span<const std::string_view> args) {
        m_empty_args = args.empty();
    }

    void HelpCommand::execute() {
        std::cout << help_message;
        if(!m_empty_args) {
            throw InvalidCommandArgumentsError{"help command takes no arguments", "tds help"};
        }
    }
}
