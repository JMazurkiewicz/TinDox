#include "tds/cli/help_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"

#include <iostream>

namespace tds::cli {
    namespace {
        inline constexpr std::string_view help_message = {
            "usage: tds <command> [args]\n"
            "\n"
            "These are TinDoxServer commands:\n"
            "* config                                                 modify server's properties\n"
            "* help                                                   display available options\n"
            "* init [dir]                                             creates server instance in current or specified "
            "directory\n"
            "* log                                                    display logs\n"
            "* run                                                    run TinDoxServer\n"
            "* user [<-p|--path> <tds-dir>] <add|remove|passwd|perms> add, modify or remove users\n"
            "* user remove <username>                                 remove specified user\n"
            "* user perms <username> [+perms] [-perms]                remove given perms of user\n"
            "\n"
            "Available permissions:\n"
            "* travel   [t] - commands like ls, pwd, cd\n"
            "* write    [w] - commands like rmdir, mkdir\n"
            "* copy     [c] - cp command\n"
            "* move     [m] - mv command\n"
            "* download [d] - dl and dlr commands\n"
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
