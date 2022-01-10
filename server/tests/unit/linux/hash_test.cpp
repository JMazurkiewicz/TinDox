#include <catch2/catch_test_macros.hpp>
#include <catch2/internal/catch_test_registry.hpp>

#include "tds/linux/hash.hpp"

using namespace tds::linux;

TEST_CASE("tds::linux::hash", "[linux]") {
    const std::string text = "crypt(3)";
    REQUIRE(hash(text) != text);
    REQUIRE(hash(text) == hash(text.c_str()));
}
