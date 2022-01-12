#include <catch2/catch_test_macros.hpp>

#include "tds/ip/socket_type.hpp"

#include <sys/socket.h>

using namespace tds::ip;

TEST_CASE("tds::ip::SocketType", "[ip]") {
    REQUIRE(static_cast<int>(SocketType::blocking) == 0);
    REQUIRE(static_cast<int>(SocketType::nonblocking) == SOCK_NONBLOCK);
}
