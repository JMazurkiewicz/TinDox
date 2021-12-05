#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll.hpp"
#include "tds/linux/signal_device.hpp"

#include <csignal>
#include <thread>

#include <sys/wait.h>
#include <unistd.h>

using namespace tds::linux;

void test_signal_device(std::invocable auto test) {
    const int fork_result = fork();

    switch(fork()) {
    case -1:
        CHECK(!"`test_signal_device` failed to create process with fork(2)");
        break;

    case 0:
        test();
        std::exit(0);
        break;

    default:
        wait(nullptr);
    }
}

TEST_CASE("tds::linux::SignalDevice", "[linux]") {
    SECTION("Test in main thread") {
        test_signal_device([] {
            SignalDevice signal_device;
            bool flag = false;

            signal_device.add_handler(SIGINT, [&](int) { flag = true; });
            signal_device.apply();

            std::raise(SIGINT);
            signal_device.handle();

            REQUIRE(flag);
        });
    }

    SECTION("Test in different thread") {
        test_signal_device([] {
            std::thread{[] {
                SignalDevice signal_device;
                bool flag = false;

                signal_device.add_handler(SIGINT, [&](int) { flag = true; });
                signal_device.apply();

                std::raise(SIGINT);
                signal_device.handle();

                REQUIRE(flag);
            }}.join();
        });
    }
}

TEST_CASE("tds::linux::{SignalDevice + Epoll}", "[linux]") {
    SECTION("Test in main thread") {
        test_signal_device([] {
            SignalDevice signal_device;
            bool flag = false;

            signal_device.add_handler(SIGINT, [&](int) { flag = true; });
            signal_device.apply();

            Epoll epoll;
            epoll.add_device(signal_device);

            std::raise(SIGINT);
            epoll.handle();

            REQUIRE(flag);
        });
    }

    SECTION("Test in different thread") {
        test_signal_device([] {
            std::thread{[] {
                SignalDevice signal_device;
                bool flag = false;

                signal_device.add_handler(SIGINT, [&](int) { flag = true; });
                signal_device.apply();

                Epoll epoll;
                epoll.add_device(signal_device);

                std::raise(SIGINT);
                epoll.handle();

                REQUIRE(flag);
            }}.join();
        });
    }
}
