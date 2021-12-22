#include <catch2/catch_test_macros.hpp>

#include "tds/user/permissions.hpp"

#include <bit>
#include <sstream>
#include <stdexcept>

using namespace tds::user;

TEST_CASE("tds::user::Permissions", "[user]") {
    using enum Permissions;

    SECTION("Test enum values") {
        REQUIRE(std::has_single_bit(static_cast<unsigned>(travel)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(write)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(copy)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(move)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(download)));
        REQUIRE(std::has_single_bit(static_cast<unsigned>(upload)));
    }

    SECTION("Test bitwise operators") {
        REQUIRE((write & travel) == Permissions{});
        REQUIRE((write ^ travel) == (write | travel));
    }

    SECTION("Test 'all' permission") {
        REQUIRE(all == (travel | write | copy | move | download | upload));
    }

    SECTION("Test 'to_string'") {
        REQUIRE(to_string(travel | copy | move) == "tcm");
        REQUIRE(to_string(upload | download) == "du");
        REQUIRE(to_string(all) == "twcmdu");
    }

    SECTION("Test 'from_string'") {
        REQUIRE(perms_from_string("dut") == (download | upload | travel));
        REQUIRE(perms_from_string("cmwt") == (copy | move | write | travel));
        REQUIRE(perms_from_string("twcmdu") == all);
        REQUIRE_THROWS_AS(perms_from_string("x"), std::runtime_error);
    }

    SECTION("Test '>>' operator") {
        std::istringstream stream{"twm"};
        Permissions perms;
        stream >> perms;
        REQUIRE(perms == (travel | write | move));
    }

    SECTION("Test '<<' operator") {
        std::ostringstream stream;
        stream << (travel | download | upload);
        REQUIRE(stream.str() == "tdu");
    }
}
