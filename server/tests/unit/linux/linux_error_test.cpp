#include <catch2/catch_test_macros.hpp>

#include "tds/linux/linux_error.hpp"

#include <cerrno>

using namespace tds::linux;

TEST_CASE("tds::linux::LinuxError", "[linux]") {
    SECTION("Test `code + string` constructor") {
        const std::string msg = "msg";
        LinuxError error{EADV, msg};

        REQUIRE(error.code().value() == EADV);
        REQUIRE(&error.code().category() == &std::system_category());
    }

    SECTION("Test `code + raw string` constructor") {
        LinuxError error{EINVAL, "msg"};

        REQUIRE(error.code().value() == EINVAL);
        REQUIRE(&error.code().category() == &std::system_category());
    }

    SECTION("Test `string` constructor") {
        const std::string msg = "msg";
        errno = EAGAIN;
        LinuxError error{"msg"};

        REQUIRE(error.code().value() == EAGAIN);
        REQUIRE(&error.code().category() == &std::system_category());
    }

    SECTION("Test `raw string` constructor") {
        errno = EUCLEAN;
        LinuxError error{"msg"};

        REQUIRE(error.code().value() == EUCLEAN);
        REQUIRE(&error.code().category() == &std::system_category());
    }
}
