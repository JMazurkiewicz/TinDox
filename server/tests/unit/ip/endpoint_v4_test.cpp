#include <catch2/catch_test_macros.hpp>

#include "tds/ip/endpoint_v4.hpp"
#include "tds/ip/ip_error.hpp"

#include <sstream>

using namespace tds::ip;

TEST_CASE("tds::ip::EndpointV4", "[ip]") {
    SECTION("Test default constructor") {
        EndpointV4 endpoint;
        REQUIRE(endpoint.get_address().as_integer() == 0);
        REQUIRE(endpoint.get_port().as_integer() == 0);
    }

    SECTION("Test address+port constructor") {
        EndpointV4 endpoint{AddressV4::localhost, Port{80}};
        REQUIRE(endpoint.get_address().as_integer() == 0x7F000001);
        REQUIRE(endpoint.get_port().as_integer() == 80);
    }

    SECTION("Test `operator<<`") {
        std::ostringstream stream;

        EndpointV4 endpoint{AddressV4::broadcast, Port{22}};
        stream << endpoint;
        REQUIRE(stream.str() == "255.255.255.255:22");

        stream.str("");
        stream << EndpointV4{AddressV4(83, 53, 123, 43), Port(443)};
        REQUIRE(stream.str() == "83.53.123.43:443");
    }

    SECTION("Test `to_string` function") {
        EndpointV4 endpoint{AddressV4(1, 1, 1, 1), Port(80)};
        REQUIRE(to_string(endpoint) == "1.1.1.1:80");
    }
}

TEST_CASE("tds::ip::make_endpoint_v4", "[ip]") {
    SECTION("Test ip string") {
        const auto first = make_endpoint_v4("0.0.0.0:11111");
        REQUIRE(first.get_address() == AddressV4::any);
        REQUIRE(first.get_port().as_integer() == 11111);

        const auto second = make_endpoint_v4("255.255.255.255:12345");
        REQUIRE(second.get_address() == AddressV4::broadcast);
        REQUIRE(second.get_port().as_integer() == 12345);

        const auto third = make_endpoint_v4("127.0.0.1:80");
        REQUIRE(third.get_address() == AddressV4::localhost);
        REQUIRE(third.get_port() == Port{80});

        const auto fourth = make_endpoint_v4("8.8.8.8:22");
        REQUIRE(fourth.get_port() == Port{22});
    }

    SECTION("Test dns string") {
        const auto endpoint1 = make_endpoint_v4("dns.google.com:80");
        REQUIRE((endpoint1.get_address() == AddressV4{8, 8, 8, 8} || endpoint1.get_address() == AddressV4{8, 8, 4, 4}));
        REQUIRE(endpoint1.get_port() == 80);

        const auto endpoint2 = make_endpoint_v4("one.one.one.one:22");
        REQUIRE((endpoint2.get_address() == AddressV4{1, 1, 1, 1} || endpoint2.get_address() == AddressV4{1, 0, 0, 1}));
        REQUIRE(endpoint2.get_port() == 22);
    }

    SECTION("Test invalid string") {
        REQUIRE_THROWS_AS(make_endpoint_v4("this will not work"), IpError);
        REQUIRE_THROWS_AS(make_endpoint_v4("1.1.1.1:invalid_port"), IpError);
        REQUIRE_THROWS_AS(make_endpoint_v4("invalid_address:80"), IpError);
        REQUIRE_THROWS_AS(make_endpoint_v4("5.5.5.256:22"), IpError);
        REQUIRE_THROWS_AS(make_endpoint_v4("5.5.5.23:90000"), IpError);
    }

    SECTION("Test invalid dns") {
        REQUIRE_THROWS_AS(make_endpoint_v4("i.don.not.exist.com:23"), IpError);
    }

    SECTION("Test localhost") {
        const auto local_http = make_endpoint_v4("localhost:80");
        REQUIRE(local_http.get_address() == AddressV4::localhost);
        REQUIRE(local_http.get_port() == 80);
    }

    SECTION("Test `to_string`, then `make_endpoint_v4`") {
        EndpointV4 endpoint{AddressV4::localhost, Port{22}};
        const std::string str = to_string(endpoint);
        EndpointV4 back_to_port = make_endpoint_v4(str);

        REQUIRE(endpoint.get_address().as_integer() == 0x7F000001);
        REQUIRE(endpoint.get_port() == 22);
        REQUIRE(str == "127.0.0.1:22");
        REQUIRE(back_to_port.get_address().as_integer() == 0x7F000001);
        REQUIRE(back_to_port.get_port() == 22);
    }
}
