#include "tds/cli/cli_error.hpp"
#include "tds/cli/command_executor.hpp"
#include "tds/cli/help_command.hpp"
#include "tds/cli/init_command.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/cli/run_command.hpp"
#include "tds/cli/user_command.hpp"
#include "tds/cli/version_command.hpp"
#include "tds/command/no_such_command_error.hpp"

#include <array>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <execinfo.h>
#include <toml++/toml.h>

[[noreturn]] inline void handle_sigsegv(int signal) {
    std::array<void*, 50> buffer;
    const size_t size = backtrace(buffer.data(), buffer.size());
    fprintf(stderr, "Got SIGSEGV\n");
    backtrace_symbols_fd(buffer.data(), size, STDERR_FILENO);
    std::exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::signal(SIGPIPE, SIG_IGN);
    std::signal(SIGSEGV, handle_sigsegv);

    using namespace tds::cli;

    if(argc <= 1) {
        HelpCommand help;
        help.execute();
    } else {
        try {
            CommandExecutor<HelpCommand, InitCommand, RunCommand, UserCommand, VersionCommand> executor;

            const std::string_view command_name = argv[1];
            executor.set_command(command_name);
            const std::vector<std::string_view> args(argv + 2, argv + argc);
            executor.parse_arguments(args);

            executor.execute();
            return EXIT_SUCCESS;
        } catch(const toml::parse_error& e) {
            std::cerr << "TOML parse error:\nDescription: " << e.description() << "\nSource file: " << e.source();
        } catch(const tds::command::NoSuchCommandError& e) {
            std::cerr << "tds: '" << e.what() << "' is not a tds command. See 'tds help'";
        } catch(const InvalidCommandExecutionError& e) {
            std::cerr << "tds: " << e.what();
        } catch(const CliError& e) {
            std::cerr << "error: '" << e.what() << '\'';
        } catch(const std::exception& e) {
            std::cerr << "fatal error: '" << e.what() << '\'';
        } catch(...) {
            std::cerr << "unknown fatal error";
        }
    }

    std::cerr << '\n';
    return EXIT_FAILURE;
}
