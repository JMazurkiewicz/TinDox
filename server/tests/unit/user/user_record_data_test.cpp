#include <catch2/catch_test_macros.hpp>

#include "tds/linux/hash.hpp"
#include "tds/user/permissions.hpp"
#include "tds/user/user_record_data.hpp"

using namespace tds::user;

TEST_CASE("tds::user::UserRecordData", "[user]") {
    SECTION("Test constructor") {
        UserRecordData data{"admin", "hash", Permissions::all};
        REQUIRE(data.get_username() == "admin");
        REQUIRE(data.get_password_hash() == "hash");
        REQUIRE(data.get_permissions() == Permissions::all);
    }

    SECTION("Test `check_password` function") {
        SECTION("Test `const std::string&` overload") {
            const std::string password = "const std::string";
            UserRecordData data{"admin", tds::linux::hash(password), Permissions::all};
            REQUIRE(data.check_password(password));
        }

        SECTION("Test `const char*` overload") {
            const char* const password = "const char* const";
            UserRecordData data{"admin", tds::linux::hash(password), Permissions::all};
            REQUIRE(data.check_password(password));
        }
    }
}
