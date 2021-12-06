#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll_device.hpp"
#include "tds/linux/signal_device.hpp"

#include <csignal>
#include <thread>

#include <sys/wait.h>
#include <unistd.h>

using namespace tds::linux;

void test_signal_device(std::invocable auto test) {
    switch(const int pid = fork()) {
    case -1:
        REQUIRE_FALSE("`test_signal_device` failed to create process with fork(2)");

    case 0:
        test();
        _exit(1);

    default: {
        int status = 1;
        waitpid(pid, &status, 0);

        REQUIRE(WIFEXITED(status));
        REQUIRE(WEXITSTATUS(status) == 0);
    }
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

            _exit(status);
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

                _exit(status);
            }}.join();
        });
    }
}

TEST_CASE("tds::linux::{SignalDevice+EpollDevice}", "[linux]") {
    SECTION("Test in main thread") {
        test_signal_device([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.apply();

            EpollDevice epoll_device;
            epoll_device.add_device(signal_device);

            std::raise(SIGINT);
            epoll_device.handle();

            _exit(status);
        });
    }

    SECTION("Test in different thread") {
        test_signal_device([] {
            std::thread{[] {
                SignalDevice signal_device;
                int status = 1;

                signal_device.add_handler(SIGINT, [&](int) { status = 0; });
                signal_device.apply();

                EpollDevice epoll_device;
                epoll_device.add_device(signal_device);

                std::raise(SIGINT);
                epoll_device.handle();

                _exit(status);
            }}.join();
        });
    }
}
