#include <catch2/catch_test_macros.hpp>

#include "tds/user/permissions.hpp"

#include <bit>
#include <sstream>
#include <stdexcept>

using namespace tds::user;

TEST_CASE("tds::user::Permissions", "[user]") {
    using enum Permissions;

    SECTION("Test enum values") {
        REQUIRE(std::has_single_bit(static_cast<unsigned>(write)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(copy)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(move)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(download)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(upload)));
    }

    SECTION("Test bitwise operators") {
        REQUIRE((write & upload) == Permissions{});
        REQUIRE((write ^ upload) == (write | upload));
    }

    SECTION("Test combo permissions") {
        REQUIRE(initial == download);
        REQUIRE(all == (write | copy | move | download | upload));
    }

    SECTION("Test 'to_string'") {
        REQUIRE(to_string(copy | move) == "cm");
        REQUIRE(to_string(upload | download) == "du");
        REQUIRE(to_string(all) == "twcmdu");
    }

    SECTION("Test 'from_string'") {
        REQUIRE(perms_from_string("du") == (download | upload));
        REQUIRE(perms_from_string("cmw") == (copy | move | write));
        REQUIRE(perms_from_string("twcmdu") == all);
        REQUIRE_THROWS_AS(perms_from_string("x"), std::runtime_error);
    }

    SECTION("Test '>>' operator") {
        std::istringstream stream{"wm"};
        Permissions perms;
        stream >> perms;
        REQUIRE(perms == (write | move));
    }

    SECTION("Test '<<' operator") {
        std::ostringstream stream;
        stream << (download | upload);
        REQUIRE(stream.str() == "du");
    }
}
