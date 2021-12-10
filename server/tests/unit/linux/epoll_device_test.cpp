#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll_device.hpp"
#include "tds/linux/linux_error.hpp"

#include <unistd.h>

using namespace tds::linux;

TEST_CASE("tds::linux::EpollDevice", "[linux]") {
    SECTION("Test valid descriptor") {
        struct Stdin : IoDevice {
            Stdin()
                : IoDevice(STDIN_FILENO) { }
        };

        auto test = [] {
            EpollDevice epoll_device;
            Stdin in;
            epoll_device.add_device(in);
        };
        REQUIRE_NOTHROW(test());
    }

    SECTION("Test invalid descriptor") {
        struct InvalidDevice : IoDevice { };

        auto test = [] {
            EpollDevice epoll_device;
            InvalidDevice invalid;
            epoll_device.add_device(invalid);
        };
        REQUIRE_THROWS_AS(test(), LinuxError);
    }
}
