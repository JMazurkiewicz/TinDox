#include <catch2/catch_test_macros.hpp>

#include "tds/ip/endpoint_v4.hpp"

#include <sstream>

#include <netinet/in.h>

using namespace tds::ip;

TEST_CASE("tds::ip::EndpointV4", "[ip]") {
    SECTION("Test default constructor") {
        EndpointV4 endpoint;
        REQUIRE(endpoint.get_address().as_integer() == 0);
        REQUIRE(endpoint.get_port().as_integer() == 0);
    }

    SECTION("Test address+port constructor") {
        EndpointV4 endpoint{AddressV4{INADDR_LOOPBACK}, Port{80}};
        REQUIRE(endpoint.get_address().as_integer() == 0x7F000001);
        REQUIRE(endpoint.get_port().as_integer() == 80);
    }

    SECTION("Test `operator<<`") {
        std::ostringstream stream;

        EndpointV4 endpoint{AddressV4{INADDR_BROADCAST}, Port{22}};
        stream << endpoint;
        REQUIRE(stream.str() == "255.255.255.255:22");

        stream.str("");
        stream << EndpointV4{AddressV4{0x53357B2B}, Port(443)};
        REQUIRE(stream.str() == "83.53.123.43:443");
    }
}
