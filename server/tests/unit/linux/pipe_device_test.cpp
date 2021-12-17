#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll_device.hpp"
#include "tds/linux/pipe_device.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <string_view>
#include <thread>

using namespace tds::linux;
using namespace std::chrono_literals;

TEST_CASE("tds::linux::Pipe{}", "[linux]") {
    SECTION("Test in main thread") {
        auto&& [read_device, write_device] = make_pipe(true);

        const std::string_view msg = "Hello";
        const ssize_t write_count = write_device.write(msg.data(), msg.size());
        REQUIRE(write_count == msg.size());

        std::array<char, 5> buffer;
        const ssize_t read_count = read_device.read(buffer.begin(), buffer.size());
        REQUIRE(read_count == msg.size());
        REQUIRE(std::ranges::equal(buffer, msg));
    }

    SECTION("Test in different thread") {
        auto&& [read_device, write_device] = make_pipe(true);

        const std::string_view msg = "Thread";
        const ssize_t write_count = write_device.write(msg.data(), msg.size());
        REQUIRE(write_count == msg.size());

        bool read_status = false;
        bool equal_status = false;

        std::thread{[&] {
            std::array<char, 6> buffer;
            const ssize_t read_count = read_device.read(buffer.begin(), buffer.size());
            read_status = (read_count == msg.size());
            equal_status = std::ranges::equal(buffer, msg);
        }}.join();

        REQUIRE(read_status);
        REQUIRE(equal_status);
    }
}

TEST_CASE("tds::linux::{Pipe{}+EpollDevice}", "[linux]") {
    SECTION("Test in main thread") {
        auto&& [read_device, write_device] = make_pipe(true);
        EpollDevice epoll_device;
        epoll_device.add_device(read_device);

        const std::string_view msg = "Epoll";
        const ssize_t write_count = write_device.write(msg.data(), msg.size());
        REQUIRE(write_count == msg.size());

        EpollBuffer buffer{2};
        epoll_device.wait_for_events(buffer);
        for(int fd : buffer.get_available_events()) {
            REQUIRE(fd == read_device.get_fd());
            std::array<char, 5> buffer;
            const ssize_t read_count = read_device.read(buffer.data(), buffer.size());
            REQUIRE(read_count == msg.size());
            REQUIRE(std::ranges::equal(msg, buffer));
        }
    }

    SECTION("Test in different thread") {
        auto&& [read_device, write_device] = make_pipe(true);
        const std::string_view msg = "Epoll";

        std::atomic_bool read_status = false;
        std::atomic_bool equal_status = false;

        std::thread reader{[&] {
            EpollDevice epoll_device;
            epoll_device.add_device(read_device);

            EpollBuffer buffer{2};
            epoll_device.wait_for_events(buffer);
            for(int fd : buffer.get_available_events()) {
                REQUIRE(fd == read_device.get_fd());
                std::array<char, 5> buffer;
                const ssize_t read_count = read_device.read(buffer.data(), buffer.size());
                read_status = (read_count == msg.size());
                equal_status = std::ranges::equal(buffer, msg);
            }
        }};

        std::this_thread::sleep_for(500ms);
        const ssize_t write_count = write_device.write(msg.data(), msg.size());
        REQUIRE(write_count == msg.size());

        std::this_thread::sleep_for(500ms);
        REQUIRE(read_status);
        REQUIRE(equal_status);
        REQUIRE(reader.joinable());
        reader.join();
    }

    SECTION("Test in main thread with nonblocking pipe and edge-triggered epoll") {
        auto&& [read_device, write_device] = make_pipe(false);
        EpollDevice epoll_device;
        epoll_device.add_device(read_device, EpollMode::edge_triggered);

        const std::string_view msg = "Epoll";
        const ssize_t write_count = write_device.write(msg.data(), msg.size());
        REQUIRE(write_count == msg.size());

        EpollBuffer buffer{2};
        epoll_device.wait_for_events(buffer);
        for(int fd : buffer.get_available_events()) {
            REQUIRE(fd == read_device.get_fd());
            std::array<char, 5> buffer;
            const ssize_t read_count = read_device.read(buffer.data(), buffer.size());
            REQUIRE(read_count == msg.size());
            REQUIRE(std::ranges::equal(msg, buffer));
        }
    }

    SECTION("Test in different thread with nonblocking pipe and edge-triggered epoll") {
        auto&& [read_device, write_device] = make_pipe(false);
        const std::string_view msg = "Epoll";

        std::atomic_bool read_status = false;
        std::atomic_bool equal_status = false;

        std::thread reader{[&] {
            EpollDevice epoll_device;
            epoll_device.add_device(read_device, EpollMode::edge_triggered);

            EpollBuffer buffer{2};
            epoll_device.wait_for_events(buffer);
            for(int fd : buffer.get_available_events()) {
                REQUIRE(fd == read_device.get_fd());
                std::array<char, 5> buffer;
                const ssize_t read_count = read_device.read(buffer.data(), buffer.size());
                read_status = (read_count == msg.size());
                equal_status = std::ranges::equal(buffer, msg);
            }
        }};

        std::this_thread::sleep_for(500ms);
        const ssize_t write_count = write_device.write(msg.data(), msg.size());
        REQUIRE(write_count == msg.size());

        std::this_thread::sleep_for(500ms);
        REQUIRE(read_status);
        REQUIRE(equal_status);
        REQUIRE(reader.joinable());
        reader.join();
    }
}
