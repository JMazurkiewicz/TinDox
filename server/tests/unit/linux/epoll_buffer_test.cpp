#include <catch2/catch_test_macros.hpp>

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"

using namespace tds::linux;

TEST_CASE("tds::linux::EpollBuffer", "[linux]") {
    EpollBuffer buffer{10};
    REQUIRE(buffer.max_size() == 10);
    REQUIRE(buffer.size() == 0);

    epoll_event* const buffer_ptr = buffer.data();
    const epoll_event* const const_buffer_ptr = std::as_const(buffer).data();
    REQUIRE(buffer_ptr != nullptr);
    REQUIRE(const_buffer_ptr != nullptr);
    REQUIRE(buffer_ptr == const_buffer_ptr);

    buffer.set_size(5);
    REQUIRE(buffer.max_size() == 10);
    REQUIRE(buffer.size() == 5);
    REQUIRE(buffer.data() == buffer_ptr);
    REQUIRE(buffer.data() == buffer_ptr);
    REQUIRE(std::as_const(buffer).data() == const_buffer_ptr);
    REQUIRE(buffer.data() == std::as_const(buffer).data());
}
