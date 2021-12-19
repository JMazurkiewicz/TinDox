#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/user_command.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::UserCommand", "[cli]") {
    SECTION("Is 'UserCommand' a valid command?") {
        REQUIRE(Command<UserCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "user";
        REQUIRE(UserCommand::name == expected);
    }
}
