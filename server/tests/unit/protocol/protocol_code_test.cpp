#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/protocol_code.hpp"

using namespace tds::protocol;

TEST_CASE("tds::protocol::ProtocolCode", "[protocol]") {
    SECTION("Test some values") {
        using enum ProtocolCode;
        REQUIRE(is_info_code(ok));
        REQUIRE(is_error_code(not_found));
        REQUIRE(is_error_code(not_logged_in));
    }

    SECTION("Is ProtocolCode C++ error code?") {
        REQUIRE(std::is_error_code_enum_v<ProtocolCode>);
    }
}
