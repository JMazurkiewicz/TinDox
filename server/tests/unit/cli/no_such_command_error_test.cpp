#include <catch2/catch_test_macros.hpp>

#include "tds/cli/no_such_command_error.hpp"

#include <algorithm>

using namespace tds::cli;

TEST_CASE("tds::cli::NoSuchCommandError", "[cli]") {
    const std::string_view name = "invalid";
    NoSuchCommandError error{name};
    const std::string_view what = error.what();
    REQUIRE(what.find("help") != std::string_view::npos);
    REQUIRE(what.find(name) != std::string_view::npos);
}
