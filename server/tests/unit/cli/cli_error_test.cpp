#include <catch2/catch_test_macros.hpp>

#include "tds/cli/cli_error.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::CliError", "[cli]") {
    SECTION("Test throwing") {
        auto throwing = [] {
            throw CliError{"throwing"};
        };

        REQUIRE_THROWS(throwing());
    }

    SECTION("Test construction") {
        const std::string msg = "example";

        CliError e{msg};
        REQUIRE(e.what() == msg);
    }
}
