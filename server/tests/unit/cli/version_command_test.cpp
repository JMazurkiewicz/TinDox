#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/version_command.hpp"

using namespace tds::cli;
using namespace std::string_view_literals;

TEST_CASE("tds::cli::VersionCommand", "[cli]") {
    SECTION("Is 'VersionCommand' a valid command?") {
        REQUIRE(Command<VersionCommand>);
    }

    SECTION("Check command name") {
        const std::string_view expected = "version";
        REQUIRE(VersionCommand::name == expected);
    }

    SECTION("Good version invocation") {
        VersionCommand version;
        REQUIRE_NOTHROW(version.parse_arguments({}));
        REQUIRE_NOTHROW(version.execute());
    }

    SECTION("Bad version invocation") {
        VersionCommand version;
        const std::array args = {"invalid"sv};
        REQUIRE_NOTHROW(version.parse_arguments(args));
        REQUIRE_THROWS_AS(version.execute(), InvalidCommandArgumentsError);
    }
}
