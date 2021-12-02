#include <catch2/catch_test_macros.hpp>

TEST_CASE("zero", "[tin.dummy.0]") {
    REQUIRE(0 == 0);
}

TEST_CASE("one", "[tin.dummy.1]") {
    REQUIRE(1 == 1);
}

TEST_CASE("two", "[tin.dummy.2]") {
    REQUIRE(2 == 2);
}
