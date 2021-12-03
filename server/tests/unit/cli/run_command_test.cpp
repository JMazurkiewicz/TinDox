#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/run_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::RunCommand", "[cli]") {
    SECTION("Is `RunCommand` a valid command?") {
        REQUIRE(Command<RunCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "run";
        REQUIRE(RunCommand::name == expected);
        REQUIRE(RunCommand::get_name() == expected);
    }
}
