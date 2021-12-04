#include <catch2/catch_test_macros.hpp>

#include "tds/ip/address_v4.hpp"
#include "tds/ip/ip_error.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <sstream>

#include <arpa/inet.h>
#include <netinet/in.h>

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

    SECTION("Test byte constrcutor") {
        AddressV4 address{128, 128, 128, 128};
        REQUIRE(address.as_integer() == 0x80808080);
    }

    SECTION("Test `as_bytes` function") {
        AddressV4 address{127, 0, 0, 1};
        constexpr std::array bytes = {127, 0, 0, 1};

        REQUIRE(std::ranges::equal(address.as_bytes() |
                                       std::views::transform([](auto b) { return std::to_integer<int>(b); }),
                                   bytes));
    }

    SECTION("Test `operator<<`") {
        std::ostringstream stream;

        AddressV4 address{127, 0, 0, 1};
        stream << address;
        REQUIRE(stream.str() == "127.0.0.1");

        stream.str("");
        stream << AddressV4{255, 255, 255, 0};
        REQUIRE(stream.str() == "255.255.255.0");
    }

    SECTION("Test predefined addresses") {
        REQUIRE(AddressV4::any.as_integer() == INADDR_ANY);
        REQUIRE(AddressV4::localhost.as_integer() == INADDR_LOOPBACK);
        REQUIRE(AddressV4::broadcast.as_integer() == INADDR_BROADCAST);
    }

    SECTION("Test `to_string` function") {
        auto address = AddressV4::broadcast;
        REQUIRE(to_string(address) == "255.255.255.255");
    }

    SECTION("Test `==` operator") {
        REQUIRE(std::equality_comparable<AddressV4>);

        const AddressV4 one{127, 0, 0, 1};
        const AddressV4 two{0x7F000001};
        const AddressV4 three{1, 0, 0, 127};

        REQUIRE(one == two);
        REQUIRE(one != three);
        REQUIRE(two != three);
    }
}

TEST_CASE("tds::ip::make_address_v4", "[ip]") {
    SECTION("Test ip string") {
        REQUIRE(make_address_v4("0.0.0.0") == AddressV4::any);
        REQUIRE(make_address_v4("255.255.255.255") == AddressV4::broadcast);
        REQUIRE(make_address_v4("127.0.0.1") == AddressV4::localhost);
        REQUIRE(make_address_v4("8.8.8.8") == AddressV4{8, 8, 8, 8});
    }

    SECTION("Test dns string") {
        const auto address1 = make_address_v4("dns.google.com");
        REQUIRE((address1 == AddressV4{8, 8, 8, 8} || address1 == AddressV4{8, 8, 4, 4}));

        const auto address2 = make_address_v4("one.one.one.one");
        REQUIRE((address2 == AddressV4{1, 1, 1, 1} || address2 == AddressV4{1, 0, 0, 1}));
    }

    SECTION("Test invalid string") {
        REQUIRE_THROWS_AS(make_address_v4("this will not work"), IpError);
        REQUIRE_THROWS_AS(make_address_v4("289.1.3.1"), IpError);
        REQUIRE_THROWS_AS(make_address_v4("255.255.255.256"), IpError);
    }

    SECTION("Test invalid dns") {
        REQUIRE_THROWS_AS(make_address_v4("i.do.not.exist.com"), IpError);
    }

    SECTION("Test localhost") {
        REQUIRE(make_address_v4("localhost") == AddressV4::localhost);
    }

    SECTION("Test `to_string`, then `make_address_v4") {
        AddressV4 address{1, 0, 0, 1};
        const std::string str = to_string(address);
        AddressV4 back_to_address = make_address_v4(str);

        REQUIRE(address.as_integer() == 0x01000001);
        REQUIRE(str == "1.0.0.1");
        REQUIRE(back_to_address.as_integer() == 0x01000001);
    }
}
