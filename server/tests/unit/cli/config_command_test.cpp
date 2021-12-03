#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/config_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::ConfigCommand", "[cli]") {
    SECTION("Is `ConfigCommand` a valid command?") {
        REQUIRE(Command<ConfigCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "config";
        REQUIRE(ConfigCommand::name == expected);
        REQUIRE(ConfigCommand::get_name() == expected);
    }
}
