#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/response.hpp"

using namespace tds::protocol;

TEST_CASE("tds::protocol::Response", "[protocol]") {
    const std::string text = "test-member-functions";
    Response response{text};
    REQUIRE(response.get_data_pointer() != nullptr);
    REQUIRE(response.get_remaining_byte_count() == text.size());

    response.add_read_byte_count(10);
    REQUIRE(response.get_remaining_byte_count() == text.size() - 10);
    REQUIRE(!response.is_fully_sent());

    response.add_read_byte_count(10);
    REQUIRE(response.get_remaining_byte_count() == text.size() - 20);
    REQUIRE(!response.is_fully_sent());

    response.add_read_byte_count(200);
    REQUIRE(response.get_remaining_byte_count() == 0);
    REQUIRE(response.is_fully_sent());
}
