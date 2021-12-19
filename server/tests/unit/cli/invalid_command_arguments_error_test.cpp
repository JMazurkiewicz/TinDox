#include <catch2/catch_test_macros.hpp>

#include "tds/cli/invalid_command_arguments_error.hpp"

using namespace tds::cli;

TEST_CASE("tds::cli::InvalidCommandArgumentsError", "[cli]") {
    const std::string what = "invalid";
    const std::string tip = "tip";

    InvalidCommandArgumentsError error{what, tip};
    REQUIRE(error.what() == what);
    REQUIRE(error.get_tip() == tip);
}
