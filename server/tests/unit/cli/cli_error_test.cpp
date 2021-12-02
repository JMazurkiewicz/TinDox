#include <catch2/catch_test_macros.hpp>

#include "tds/cli/cli_error.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::cli_error", "[cli]") {
    SECTION("Test throwing") {
        auto throwing = [] {
            throw cli_error{"throwing"};
        };

        REQUIRE_THROWS(throwing());
    }

    SECTION("Test construction") {
        const std::string msg = "example";

        cli_error e{msg};
        REQUIRE(e.what() == msg);
    }
}
