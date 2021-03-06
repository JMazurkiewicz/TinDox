#include <catch2/catch_test_macros.hpp>

#include "tds/cli/cli_error.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::CliError", "[cli]") {
    SECTION("Test throwing") {
        auto throwing = [] {
            throw CliError{"CliError"};
        };

        REQUIRE_THROWS_AS(throwing(), CliError);
    }

    SECTION("Test relations") {
        REQUIRE(std::derived_from<CliError, std::runtime_error>);
        REQUIRE(std::derived_from<CliError, std::exception>);
    }

    SECTION("Test construction") {
        const char* const raw_msg = "example";
        const std::string msg = raw_msg;
        REQUIRE(CliError{raw_msg}.what() == msg);
        REQUIRE(CliError{msg}.what() == msg);
    }
}
