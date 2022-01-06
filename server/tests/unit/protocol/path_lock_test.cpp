#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/path_lock.hpp"

#include <stdexcept>
#include <type_traits>

using namespace tds::protocol;

TEST_CASE("tds::protocol::PathLock", "[protocol]") {
    const std::string path = "/home/td/server";

    SECTION("Test construction") {
        PathLock lock{path};
        REQUIRE(lock.get_locked_path() == path);
    }

    SECTION("Test bad construction") {
        REQUIRE_THROWS_AS(PathLock{"../server"}, std::runtime_error);
    }

    SECTION("Test deleted functions") {
        REQUIRE(!std::is_copy_constructible_v<PathLock>);
        REQUIRE(!std::is_copy_assignable_v<PathLock>);
    }

    SECTION("Test path lock checking") {
        PathLock lock{path};
        REQUIRE(lock.has_locked_path(path));
        REQUIRE(lock.has_locked_path(path + "/subdirectory"));
        REQUIRE(!lock.has_locked_path("/home"));
        REQUIRE(!lock.has_locked_path("/home/td/client"));
        REQUIRE_THROWS_AS(lock.has_locked_path("../relative_path"), std::runtime_error);
    }

    SECTION("Test 'make_path_lock'") {
        auto lock_ptr = make_path_lock(path);
        REQUIRE(lock_ptr->has_locked_path(path));
        REQUIRE(lock_ptr->has_locked_path(path + "/subdirectory"));
        REQUIRE(!lock_ptr->has_locked_path("/home"));
        REQUIRE(!lock_ptr->has_locked_path("/home/td/client"));
        REQUIRE_THROWS_AS(lock_ptr->has_locked_path("../relative_path"), std::runtime_error);
    }
}
