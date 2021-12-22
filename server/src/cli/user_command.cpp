#include "tds/cli/user_command.hpp"

#include "tds/cli/command_executor.hpp"
#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/cli/user_commands/user_add_command.hpp"
#include "tds/cli/user_commands/user_passwd_command.hpp"
#include "tds/cli/user_commands/user_perms_command.hpp"
#include "tds/cli/user_commands/user_remove_command.hpp"
#include "tds/user/user_table.hpp"

#include <filesystem>

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::cli {
    void UserCommand::parse_arguments(std::span<const std::string_view> args) {
        if(const auto size = args.size(); size == 0) {
            throw InvalidCommandArgumentsError{"tds user takes at least one argument",
                                               "tds user <add|passwd|remove|perms>"};
        } else if(args.size() > 2 && (args.front() == "-d" || args.front() == "--dir")) {
            load_user_table(args[1]);
            m_subcommand = args[2];
            m_subcommand_args = args.subspan(3);
        } else {
            load_user_table(fs::current_path());
            m_subcommand = args[0];
            m_subcommand_args = args.subspan(1);
        }
    }

    void UserCommand::execute() {
        using namespace user_commands;
        CommandExecutor<UserAddCommand, UserPasswdCommand, UserPermsCommand, UserRemoveCommand> executor;
        executor.set_command(m_subcommand);
        executor.parse_arguments(m_subcommand_args);
        executor.visit_command([this](auto& command) { command.set_user_table(m_user_table); });
        executor.execute();
        m_user_table.commit_table();
    }

    void UserCommand::load_user_table(std::filesystem::path path) {
        path /= ".tds/users";
        if(!fs::exists(path)) {
            throw InvalidCommandExecutionError{fmt::format("could not find file {}", path)};
        } else {
            m_user_table.open(std::move(path));
        }
    }
}
