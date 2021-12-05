#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll.hpp"
#include "tds/linux/linux_error.hpp"

#include <unistd.h>

using namespace tds::linux;
using namespace std::chrono;

TEST_CASE("tds::linux::Epoll", "[linux]") {
    struct TestDevice : IoDevice {
        void handle() override { }
    };

    SECTION("Test valid descriptor") {
        auto test = [] {
            struct Stdin : TestDevice {
                Stdin() {
                    set_fd(STDIN_FILENO);
                }
            };

            Epoll epoll;
            Stdin in;
            epoll.add_device(in);
        };
        REQUIRE_NOTHROW(test());
    }

    SECTION("Test invalid descriptor") {
        struct InvalidDevice : TestDevice { };

        auto test = [] {
            Epoll epoll;
            InvalidDevice invalid;
            epoll.add_device(invalid);
        };
        REQUIRE_THROWS_AS(test(), LinuxError);
    }
}
