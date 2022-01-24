#include <catch2/catch_test_macros.hpp>

#include "tds/ip/port.hpp"

#include <sstream>

using namespace tds::ip;

TEST_CASE("tds::ip::Port", "[ip]") {
    SECTION("Test default constructor") {
        Port port;
        REQUIRE(port.as_integer() == 0);
        REQUIRE(Port{}.as_integer() == 0);
    }

    SECTION("Test integer constructor") {
        Port port{80};
        REQUIRE(port.as_integer() == 80);
    }

    SECTION("Test `operator<<`") {
        std::ostringstream stream;

        Port port{541};
        stream << port;
        REQUIRE(stream.str() == "541");

        stream.str("");
        stream << Port{30'000};
        REQUIRE(stream.str() == "30000");
    }

    SECTION("Test predefined ports") {
        REQUIRE(Port::invalid.as_integer() == 0);
    }

    SECTION("Test `==` operator") {
        REQUIRE(std::equality_comparable<Port>);

        const Port example{22};
        REQUIRE(example == Port{22});
        REQUIRE(example != Port{80});
    }
}
