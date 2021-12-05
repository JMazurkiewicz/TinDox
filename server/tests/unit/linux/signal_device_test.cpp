#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll.hpp"
#include "tds/linux/signal_device.hpp"

#include <csignal>
#include <thread>

#include <sys/wait.h>
#include <unistd.h>

using namespace tds::linux;

void test_signal_device(std::invocable auto test) {
    switch(fork()) {
    case -1:
        REQUIRE(!"`test_signal_device` failed to create process with fork(2)");
        break;

    case 0:
        test();
        REQUIRE(!"Should be unreachable");

    default:
        int result = -1;
        wait(&result);
        REQUIRE(result == 0);
    }
}

TEST_CASE("tds::linux::SignalDevice", "[linux]") {
    SECTION("Test in main thread") {
        test_signal_device([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.apply();

            std::raise(SIGINT);
            signal_device.handle();

            std::exit(status);
        });
    }

    SECTION("Test in different thread") {
        test_signal_device([] {
            std::thread{[] {
                SignalDevice signal_device;
                int status = 1;

                signal_device.add_handler(SIGINT, [&](int) { status = 0; });
                signal_device.apply();

                std::raise(SIGINT);
                signal_device.handle();

                std::exit(status);
            }}.join();
        });
    }
}

TEST_CASE("tds::linux::{SignalDevice+Epoll}", "[linux]") {
    SECTION("Test in main thread") {
        test_signal_device([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.apply();

            Epoll epoll;
            epoll.add_device(signal_device);

            std::raise(SIGINT);
            epoll.handle();

            std::exit(status);
        });
    }

    SECTION("Test in different thread") {
        test_signal_device([] {
            std::thread{[] {
                SignalDevice signal_device;
                int status = 1;

                signal_device.add_handler(SIGINT, [&](int) { status = 0; });
                signal_device.apply();

                Epoll epoll;
                epoll.add_device(signal_device);

                std::raise(SIGINT);
                epoll.handle();

                std::exit(status);
            }}.join();
        });
    }
}
