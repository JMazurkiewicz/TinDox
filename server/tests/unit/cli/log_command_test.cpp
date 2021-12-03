#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/log_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::log_command", "[cli]") {
    SECTION("Is `log_command` a valid command?") {
        REQUIRE(command<log_command>);
    }

    SECTION("Check name") {
        REQUIRE(log_command::name() == "log");
    }
}
