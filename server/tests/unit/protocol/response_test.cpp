#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/response.hpp"

using namespace tds::protocol;

TEST_CASE("tds::protocol::Response", "[protocol]") {
    SECTION("Test member functions") {
        Response response{Code::ok, "ls"};
        REQUIRE(response.get_current_data() != nullptr);

        response.add_line("file1");
        response.add_line("file2");
        response.add_line("file3");
        response.finish();

        constexpr std::size_t expected_size = 26;
        REQUIRE(response.get_remaining_byte_count() == expected_size);

        response.add_offset(10);
        REQUIRE(response.get_offset() == 10);
        REQUIRE(response.get_remaining_byte_count() == 16);
        REQUIRE(!response.is_fully_sent());

        response.add_offset(16);
        REQUIRE(response.get_offset() == expected_size);
        REQUIRE(response.get_remaining_byte_count() == 0);
        REQUIRE(response.is_fully_sent());
    }

    SECTION("Test for unsigned integer underflow") {
        Response response{Code::invalid_field, "ls"};
        REQUIRE(response.get_current_data() != nullptr);
        response.finish();
        constexpr std::size_t expected_size = 8;
        REQUIRE(response.get_remaining_byte_count() == expected_size);

        response.add_offset(3);
        REQUIRE(response.get_offset() == 3);
        REQUIRE(response.get_remaining_byte_count() == 5);
        REQUIRE(!response.is_fully_sent());

        response.add_offset(16);
        REQUIRE(response.get_offset() == expected_size);
        REQUIRE(response.get_remaining_byte_count() == 0);
        REQUIRE(response.is_fully_sent());
    }
}
