#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/help_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::help_command", "[cli]") {
    SECTION("Is `help_command` a valid command?") {
        REQUIRE(command<help_command>);
    }

    SECTION("Check name") {
        REQUIRE(help_command::name() == "help");
    }
}
