#include <catch2/catch_test_macros.hpp>

#include "tds/linux/io_device.hpp"

#include <unistd.h>

using namespace tds::linux;

TEST_CASE("tds::linux::IoDevice", "[linux]") {
    struct TestDevice : IoDevice {
        using IoDevice::IoDevice;

        explicit TestDevice(int fd)
            : IoDevice(fd) { }

        using IoDevice::set_fd;
    };

    SECTION("Test default constructor") {
        TestDevice dev;
        REQUIRE(dev.get_fd() == -1);
        REQUIRE(!dev.is_valid());
        REQUIRE(!dev);
    }

    SECTION("Test `set_fd` function") {
        TestDevice dev;
        dev.set_fd(STDIN_FILENO);

        REQUIRE(dev.get_fd() == STDIN_FILENO);
        REQUIRE(dev.is_valid());
        REQUIRE(dev);
    }

    SECTION("Test `release` function") {
        TestDevice dev{0};
        const int fd = dev.release();
        REQUIRE(fd == 0);
        REQUIRE(dev.get_fd() == -1);
        REQUIRE(!dev);
    }

    SECTION("Test `==` operator") {
        TestDevice dev1{0};
        TestDevice dev2{0};
        TestDevice dev3{1};

        REQUIRE(dev1 == dev2);
        REQUIRE(dev2 != dev3);
        REQUIRE(dev1 != dev3);
    }
}
