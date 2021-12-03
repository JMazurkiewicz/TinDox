#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/log_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::LogCommand", "[cli]") {
    SECTION("Is `LogCommand` a valid command?") {
        REQUIRE(Command<LogCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "log";
        REQUIRE(LogCommand::name == expected);
        REQUIRE(LogCommand::get_name() == expected);
    }
}
