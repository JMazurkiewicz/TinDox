#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/auth_token.hpp"

#include <type_traits>

using namespace tds::protocol;
using enum tds::user::Permissions;

TEST_CASE("tds::protocol::AuthToken", "[protocol]") {
    SECTION("Test construction") {
        const std::string username = "admin";
        const AuthToken token{username, all};
        REQUIRE(token.get_username() == username);
        REQUIRE(token.get_perms() == all);
    }

    SECTION("Test type properties") {
        REQUIRE(!std::is_copy_constructible_v<AuthToken>);
        REQUIRE(!std::is_copy_assignable_v<AuthToken>);
        REQUIRE(!std::copyable<AuthToken>);
    }
}
