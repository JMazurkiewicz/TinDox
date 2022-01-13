#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/response.hpp"
#include "tds/protocol/response_builder.hpp"

using namespace tds::protocol;
using namespace std::string_view_literals;

TEST_CASE("tds::protocol::ResponseBuilder", "[protocol]") {
    SECTION("Test response without lines") {
        ResponseBuilder builder;
        builder.set_code(ProtocolCode::ok);
        builder.set_command_name("example"sv);

        const Response response = builder.get_response();
        REQUIRE(response.get_data_pointer() == "100 example\n\n"sv);
    }

    SECTION("Test response with lines") {
        ResponseBuilder builder;
        builder.set_code(ProtocolCode::ok);
        builder.set_command_name("example2"sv);
        builder.add_line("the line");

        const Response response = builder.get_response();
        REQUIRE(response.get_data_pointer() == "100 example2\nthe line\n\n"sv);
    }

    SECTION("Test type properties") {
        REQUIRE(!std::is_copy_constructible_v<ResponseBuilder>);
        REQUIRE(!std::is_copy_assignable_v<ResponseBuilder>);
        REQUIRE(!std::copyable<ResponseBuilder>);
    }
}
