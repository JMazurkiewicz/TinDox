#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/config_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::config_command", "[cli]") {
    SECTION("Is `config_command` a valid command?") {
        REQUIRE(command<config_command>);
    }
}
