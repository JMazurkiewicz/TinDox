#include "tds/cli/cli_error.hpp"
#include "tds/cli/command_executor.hpp"
#include "tds/cli/help_command.hpp"
#include "tds/cli/init_command.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/cli/run_command.hpp"
#include "tds/cli/user_command.hpp"
#include "tds/command/no_such_command_error.hpp"

#include <array>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <execinfo.h>
#include <toml++/toml.h>

void handle_sigsegv(int signal) {
    std::array<void*, 50> buffer;
    const size_t size = backtrace(buffer.data(), buffer.size());
    fprintf(stderr, "Got SIGSEGV\n");
    backtrace_symbols_fd(buffer.data(), size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);
    std::signal(SIGPIPE, SIG_IGN);
    std::signal(SIGSEGV, handle_sigsegv);

    if(argc <= 1) {
        tds::cli::HelpCommand help;
        help.execute();
    } else {
        try {
            const std::vector<std::string_view> argument_vector(argv + 2, argv + argc);

            using namespace tds::cli;
            using Executor = CommandExecutor<HelpCommand, InitCommand, RunCommand, UserCommand>;

            Executor executor;
            executor.set_command(argv[1]);
            executor.parse_arguments(argument_vector);
            executor.execute();
            return EXIT_SUCCESS;
        } catch(const toml::parse_error& e) {
            std::cerr << "TOML parse error:\n"
                         "Description: "
                      << e.description()
                      << "\n"
                         "Source file: "
                      << e.source() << '\n';
        } catch(const tds::command::NoSuchCommandError& e) {
            std::cerr << "tds: '" << e.what() << "' is not a tds command. See 'tds help'" << '\n';
        } catch(const tds::cli::InvalidCommandExecutionError& e) {
            std::cerr << "tds: " << e.what() << '\n';
        } catch(const tds::cli::CliError& e) {
            std::cerr << "error: '" << e.what() << "'\n";
        } catch(const std::exception& e) {
            std::cerr << "fatal error: '" << e.what() << "'\n";
        } catch(...) {
            std::cerr << "Unknown fatal error!\n";
        }
    }

    return EXIT_FAILURE;
}
