#include <catch2/catch_test_macros.hpp>

#include "hello.hpp"

TEST_CASE("tin::hello()", "[tin.dummy]") {
    REQUIRE(tin::hello() == "Hello TinDox!");
}
