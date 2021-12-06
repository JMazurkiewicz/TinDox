#include <catch2/catch_test_macros.hpp>
#include <unit.hpp>

#include "tds/linux/epoll_device.hpp"
#include "tds/linux/signal_device.hpp"

#include <csignal>
#include <thread>

using namespace tds::linux;

TEST_CASE("tds::linux::SignalDevice", "[linux]") {
    SECTION("Test in main thread") {
        tds::unit::test_in_new_process([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.apply();

            std::raise(SIGINT);
            signal_device.handle();

            return status;
        });
    }

    SECTION("Test in different thread") {
        tds::unit::test_in_new_process([] {
            std::thread{[] {
                SignalDevice signal_device;
                int status = 1;

                signal_device.add_handler(SIGINT, [&](int) { status = 0; });
                signal_device.apply();

                std::raise(SIGINT);
                signal_device.handle();

                _exit(status);
            }}.join();

            return 1;
        });
    }
}

TEST_CASE("tds::linux::{SignalDevice+EpollDevice}", "[linux]") {
    SECTION("Test in main thread") {
        tds::unit::test_in_new_process([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.apply();

            EpollDevice epoll_device;
            epoll_device.add_device(signal_device);

            std::raise(SIGINT);
            epoll_device.handle();

            return status;
        });
    }

    SECTION("Test in different thread") {
        tds::unit::test_in_new_process([] {
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

            return 1;
        });
    }
}
