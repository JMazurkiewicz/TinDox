#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/init_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::InitCommand", "[cli]") {
    SECTION("Is `InitCommand` a valid command?") {
        REQUIRE(Command<InitCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "init";
        REQUIRE(InitCommand::name == expected);
        REQUIRE(InitCommand::get_name() == expected);
    }
}
