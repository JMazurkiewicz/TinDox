#include <catch2/catch_test_macros.hpp>

using namespace tds::protocol;

consteval bool is_info_code(ProtocolCode code) noexcept {
    return static_cast<int>(code) / 100 == 1;
}

consteval bool is_frame_error_code(ProtocolCode code) noexcept {
    return static_cast<int>(code) / 100 == 3;
}

consteval bool is_error_code(ProtocolCode code) noexcept {
    return static_cast<int>(code) / 100 == 4;
}

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
