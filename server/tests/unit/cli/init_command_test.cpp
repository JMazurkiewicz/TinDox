#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/init_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::init_command", "[cli]") {
    SECTION("Is `init_command` a valid command?") {
        REQUIRE(command<init_command>);
    }

    SECTION("Check name") {
        REQUIRE(init_command::name() == "init");
    }
}
