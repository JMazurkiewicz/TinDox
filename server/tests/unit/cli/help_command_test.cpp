#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/help_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::HelpCommand", "[cli]") {
    SECTION("Is `HelpCommand` a valid command?") {
        REQUIRE(Command<HelpCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "help";
        REQUIRE(HelpCommand::name == expected);
        REQUIRE(HelpCommand::get_name() == expected);
    }
}
