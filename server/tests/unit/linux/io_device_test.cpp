#include <catch2/catch_test_macros.hpp>

#include "tds/linux/io_device.hpp"

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
    }

    SECTION("Test `set_fd` function") {
        TestDevice dev;
        dev.set_fd(0);

        REQUIRE(dev.get_fd() == 0);
        REQUIRE(dev.is_valid());
    }
}
