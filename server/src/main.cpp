#include "tds/cli/command_runner.hpp"
#include "tds/cli/config_command.hpp"
#include "tds/cli/help_command.hpp"
#include "tds/cli/init_command.hpp"
#include "tds/cli/log_command.hpp"
#include "tds/cli/run_command.hpp"
#include "tds/cli/user_command.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

namespace {
    void start(std::span<const std::string_view> args) {
        using namespace tds;

        using CommandRunner = cli::CommandRunner<
            cli::ConfigCommand,
            cli::HelpCommand,
            cli::InitCommand,
            cli::LogCommand,
            cli::RunCommand,
            cli::UserCommand
        >;

        CommandRunner runner;
        runner.run(args.front(), args.subspan(1));
    }
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    int exit_status = EXIT_SUCCESS;

    if(argc <= 1) {
        tds::cli::HelpCommand help;
        help.execute({});
        exit_status = EXIT_FAILURE;
    } else {
        try {
            std::vector<std::string_view> args(argv + 1, argv + argc);
            start(args);
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            exit_status = EXIT_FAILURE;
        }
    }

    return exit_status;
}
