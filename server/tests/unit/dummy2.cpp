#include <catch2/catch_test_macros.hpp>

#include "tds/hello.hpp"

TEST_CASE("tds::hello()", "[tin.dummy]") {
    REQUIRE(tds::hello() == "Hello TinDox!");
}
