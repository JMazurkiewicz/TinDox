#include <catch2/catch_test_macros.hpp>

#include "tds/ip/ip_error.hpp"
#include "tds/ip/port.hpp"

#include <algorithm>
#include <ranges>
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

    SECTION("Test byte constructor") {
        Port port{0xAA, 0xBB};
        REQUIRE(port.as_integer() == 0xAABB);
    }

    SECTION("Test `as_bytes` function") {
        Port port{0xCC, 0xDD};
        constexpr std::array bytes = {0xCC, 0xDD};

        REQUIRE(
            std::ranges::equal(port.as_bytes() | std::views::transform([](auto b) { return std::to_integer<int>(b); }),
                               bytes));
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
        REQUIRE(Port::default_port.as_integer() == 37666);
    }

    SECTION("Test `==` operator") {
        REQUIRE(std::equality_comparable<Port>);
        REQUIRE(requires(Port p1, std::uint16_t p2) {
            { p1 == p2 } -> std::same_as<bool>;
            { p1 != p2 } -> std::same_as<bool>;
        });

        const Port one{10};
        const Port two{0x00, 0x0A};
        const Port three{0x0A, 0x00};

        REQUIRE(one == two);
        REQUIRE(two != three);
        REQUIRE(one != three);
    }

    SECTION("Test `to_string` function") {
        Port port{420};
        REQUIRE(to_string(port) == "420");
    }

    SECTION("Test `to_string`, then `make_port`") {
        Port port{22};
        const std::string str = to_string(port);
        Port back_to_port = make_port(str);

        REQUIRE(port == 22);
        REQUIRE(str == "22");
        REQUIRE(back_to_port == 22);
    }

    SECTION("Test `make_port` function") {
        const char* good = "40555";
        const char* bad1 = "I am bad";
        const char* bad2 = "123456";
        const char* bad3 = "80invalid-chars";

        REQUIRE(make_port(good).as_integer() == 40555);
        REQUIRE_THROWS_AS(make_port(bad1), IpError);
        REQUIRE_THROWS_AS(make_port(bad2), IpError);
        REQUIRE_THROWS_AS(make_port(bad3), IpError);
    }
}
