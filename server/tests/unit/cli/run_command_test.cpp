#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/cli/run_command.hpp"

#include <array>

using namespace tds::cli;
using namespace std::string_view_literals;

TEST_CASE("tds::cli::RunCommand", "[cli]") {
    SECTION("Is 'RunCommand' a valid command?") {
        REQUIRE(Command<RunCommand>);
    }

    SECTION("Check name") {
        REQUIRE(RunCommand::name == "run");
    }

    SECTION("Test running with port argument without value (long form)") {
        RunCommand command;
        const std::array args = {"--port"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }

    SECTION("Test running with port argument without value (short form)") {
        RunCommand command;
        const std::array args = {"-p"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }

    SECTION("Test running with tds config directory argument without value (long form)") {
        RunCommand command;
        const std::array args = {"--dir"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }

    SECTION("Test running with tds config directory argument without value (short form)") {
        RunCommand command;
        const std::array args = {"-d"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }

    SECTION("Test running with invalid port (long form)") {
        RunCommand command;
        const std::array args = {"--port"sv, "-5"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }

    SECTION("Test running with invalid port (short form)") {
        RunCommand command;
        const std::array args = {"-p"sv, "80000"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }

    SECTION("Test running with invalid tds config directory (long form)") {
        RunCommand command;
        const std::array args = {"--dir"sv, "/invalid/path"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandExecutionError);
    }

    SECTION("Test running with invalid tds config directory (short form)") {
        RunCommand command;
        const std::array args = {"-d"sv, "/i/p"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandExecutionError);
    }

    SECTION("Test running with invalid argument") {
        RunCommand command;
        const std::array args = {"--invalid"sv};
        REQUIRE_THROWS_AS(command.parse_arguments(args), InvalidCommandArgumentsError);
    }
}
