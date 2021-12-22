#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/help_command.hpp"
#include "tds/cli/invalid_command_arguments_error.hpp"

#include <array>

using namespace tds::cli;
using namespace std::string_view_literals;

TEST_CASE("tds::cli::HelpCommand", "[cli]") {
    SECTION("Is 'HelpCommand' a valid command?") {
        REQUIRE(Command<HelpCommand>);
    }

    SECTION("Check command name") {
        const std::string_view expected = "help";
        REQUIRE(HelpCommand::name == expected);
    }

    SECTION("Good help invocation") {
        HelpCommand help;
        REQUIRE_NOTHROW(help.parse_arguments({}));
        REQUIRE_NOTHROW(help.execute());
    }

    SECTION("Bad help invocation") {
        HelpCommand help;
        const std::array args = {"invalid"sv};
        REQUIRE_NOTHROW(help.parse_arguments(args));
        REQUIRE_THROWS_AS(help.execute(), InvalidCommandArgumentsError);
    }
}
