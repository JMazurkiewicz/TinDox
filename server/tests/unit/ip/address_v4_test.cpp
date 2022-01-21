#include <catch2/catch_test_macros.hpp>

#include "tds/ip/address_v4.hpp"

#include <sstream>

#include <arpa/inet.h>

using namespace tds::ip;

TEST_CASE("tds::ip::AddressV4", "[ip]") {
    SECTION("Test default constructor") {
        AddressV4 address;
        REQUIRE(address.as_integer() == 0);
        REQUIRE(AddressV4{}.as_integer() == 0);
    }

    SECTION("Test integer constructor") {
        AddressV4 address{0x10203040};
        REQUIRE(address.as_integer() == 0x10203040);
    }

    SECTION("Test `operator<<`") {
        std::ostringstream stream;

        AddressV4 address{INADDR_LOOPBACK};
        stream << address;
        REQUIRE(stream.str() == "127.0.0.1");

        stream.str("");
        stream << AddressV4{0xFFFFFF00};
        REQUIRE(stream.str() == "255.255.255.0");
    }

    SECTION("Test predefined addresses") {
        REQUIRE(AddressV4::any.as_integer() == INADDR_ANY);
    }

    SECTION("Test `==` operator") {
        REQUIRE(std::equality_comparable<AddressV4>);

        const AddressV4 one{INADDR_LOOPBACK};
        const AddressV4 two{0x7F000001};
        const AddressV4 three{0x0100007F};

        REQUIRE(one == two);
        REQUIRE(one != three);
        REQUIRE(two != three);
    }
}
