#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/user_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::user_command", "[cli]") {
    SECTION("Is `user_command` a valid command?") {
        REQUIRE(command<user_command>);
    }
}
