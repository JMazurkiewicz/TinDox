#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/signal_device.hpp"

#include <csignal>
#include <thread>

#include <sys/wait.h>

using namespace tds::linux;

template<typename T>
    requires(std::invocable<T>&& std::same_as<std::invoke_result_t<T>, int>)
void test_in_new_process(T&& test) {
    if(const int pid = fork(); pid == -1) {
        REQUIRE_FALSE("`test_in_new_process` failed to create process with fork(2)");
    } else if(pid == 0) {
        const int status = std::invoke(std::forward<T>(test));
        _exit(status);
    } else {
        int status = 1;
        waitpid(pid, &status, 0);

        REQUIRE(WIFEXITED(status));
        REQUIRE(WEXITSTATUS(status) == 0);
    }
}

TEST_CASE("tds::linux::SignalDevice", "[linux]") {
    SECTION("Test in main thread") {
        test_in_new_process([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.start_signal_device();

            std::raise(SIGINT);
            signal_device.handle_signal();

            return status;
        });
    }

    SECTION("Test in different thread") {
        test_in_new_process([] {
            std::thread{[] {
                SignalDevice signal_device;
                int status = 1;

                signal_device.add_handler(SIGINT, [&](int) { status = 0; });
                signal_device.start_signal_device();

                std::raise(SIGINT);
                signal_device.handle_signal();

                _exit(status);
            }}.join();

            return 1;
        });
    }
}

TEST_CASE("tds::linux::{SignalDevice+EpollDevice}", "[linux]") {
    SECTION("Test in main thread") {
        test_in_new_process([] {
            SignalDevice signal_device;
            int status = 1;

            signal_device.add_handler(SIGINT, [&](int) { status = 0; });
            signal_device.start_signal_device();

            EpollDevice epoll_device;
            epoll_device.add_device(signal_device, EventType::in);
            std::raise(SIGINT);

            EpollBuffer buffer{4};
            epoll_device.wait_for_events(buffer);
            for(auto [fd, events] : buffer.get_events()) {
                REQUIRE(fd == signal_device.get_fd());
                signal_device.handle_signal();
            }

            return status;
        });
    }

    SECTION("Test in different thread") {
        test_in_new_process([] {
            std::thread{[] {
                SignalDevice signal_device;
                int status = 1;

                signal_device.add_handler(SIGINT, [&](int) { status = 0; });
                signal_device.start_signal_device();

                EpollDevice epoll_device;
                epoll_device.add_device(signal_device, EventType::in);
                std::raise(SIGINT);

                EpollBuffer buffer{4};
                epoll_device.wait_for_events(buffer);
                for(auto [fd, events] : buffer.get_events()) {
                    REQUIRE(fd == signal_device.get_fd());
                    signal_device.handle_signal();
                }

                _exit(status);
            }}.join();

            return 1;
        });
    }
}
