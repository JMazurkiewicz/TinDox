#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/field_value.hpp"
#include "tds/protocol/request.hpp"

using namespace tds::protocol;

TEST_CASE("tds::protocol::Request", "[protocol]") {
    SECTION("Test name") {
        const std::string_view name = "pwd";
        Request request{std::string{name}};
        REQUIRE(request.get_name() == name);
    }

    SECTION("Test adding fields") {
        Request request{"fields"};
        request.add_field(Field{"field0", 123u});
        request.add_field(Field{"field1", 321U});

        for(auto&& field : request.get_fields()) {
            REQUIRE(field.get_name().starts_with("field"));
            REQUIRE(field.get_value_type() == FieldValueType::integer);
        }
    }
}
