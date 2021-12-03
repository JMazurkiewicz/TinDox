#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/run_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::run_command", "[cli]") {
    SECTION("Is `run_command` a valid command?") {
        REQUIRE(command<run_command>);
    }

    SECTION("Check name") {
        REQUIRE(run_command::name() == "run");
    }
}
