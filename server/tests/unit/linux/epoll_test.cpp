#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll.hpp"
#include "tds/linux/io_device.hpp"
#include "tds/linux/linux_error.hpp"

#include <chrono>
#include <cstdio>

#include <fcntl.h>
#include <unistd.h>

using namespace tds::linux;
using namespace std::chrono;

struct TestDevice : IoDevice {
    void handle() override { }
};

struct Stdin : TestDevice {
    Stdin() {
        set_fd(STDIN_FILENO);
    }
};

struct InvalidDevice : TestDevice { };

TEST_CASE("tds::linux::Epoll", "[linux]") {
    SECTION("Test valid descriptor") {
        auto test = [] {
            Epoll epoll;
            Stdin in;
            epoll.add_device(in);
        };
        REQUIRE_NOTHROW(test());
    }

    SECTION("Test invalid descriptor") {
        auto test = [] {
            Epoll epoll;
            InvalidDevice invalid;
            epoll.add_device(invalid);
        };
        REQUIRE_THROWS_AS(test(), LinuxError);
    }
}
