#include <catch2/catch_test_macros.hpp>

#include "tds/linux/event_type.hpp"

#include <sys/epoll.h>

using namespace tds::linux;
using enum EventType;

TEST_CASE("tds::linux::EventType", "[linux]") {
    SECTION("Test ~ operator") {
        REQUIRE(static_cast<int>(~in) == ~EPOLLIN);
    }

    SECTION("Test | operator") {
        REQUIRE(static_cast<int>(in | out) == (EPOLLIN | EPOLLOUT));
        REQUIRE(static_cast<int>(one_shot | edge_triggered) == (EPOLLONESHOT | EPOLLET));
    }

    SECTION("Test |= operator") {
        EventType events = in | out;
        events |= edge_triggered;
        REQUIRE(static_cast<int>(events) == (EPOLLIN | EPOLLOUT | EPOLLET));
    }

    SECTION("Test & operator") {
        const EventType events = in | out | one_shot;
        REQUIRE(static_cast<int>(events & in) == EPOLLIN);
    }

    SECTION("Test &= operator") {
        EventType events = in | one_shot | edge_triggered;
        events &= one_shot;
        REQUIRE(static_cast<int>(events) == EPOLLONESHOT);
    }

    SECTION("Test ^ operator") {
        REQUIRE(static_cast<int>(in ^ out) == (EPOLLIN ^ EPOLLOUT));
        REQUIRE(static_cast<int>((in | out) ^ out) == EPOLLIN);
    }

    SECTION("Test ^= operator") {
        EventType events = in | out | one_shot;
        events ^= out | edge_triggered;
        REQUIRE(static_cast<int>(events) == (EPOLLIN | EPOLLONESHOT | EPOLLET));
    }
}
