#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "tds/protocol/field.hpp"
#include "tds/protocol/field_value.hpp"
#include "tds/protocol/protocol_error.hpp"

using namespace tds::protocol;

TEST_CASE("tds::protocol::Field", "[protocol]") {
    SECTION("Test empty field") {
        Field field{"null", {}};
        REQUIRE(field.get_name() == "null");
        REQUIRE(field.get_value_type() == FieldValueType::none);

        field.visit([&]<typename T>(T&& value) { REQUIRE(std::same_as<std::remove_cvref_t<T>, std::monostate>); });
    }

    SECTION("Test boolean field") {
        constexpr bool value = true;

        Field field{"bool", value};
        REQUIRE(field.get_name() == "bool");
        REQUIRE(field.get_value_type() == FieldValueType::boolean);
        REQUIRE(std::get<bool>(field.get_value()) == value);

        auto opt = field.get_boolean();
        REQUIRE(opt.has_value());
        REQUIRE(*opt == value);

        field.visit([&]<typename T>(T&& value) { REQUIRE(std::same_as<std::remove_cvref_t<T>, bool>); });
    }

    SECTION("Test int field") {
        constexpr std::uintmax_t value = 0xDEADBEEF;

        Field field{"int", value};
        REQUIRE(field.get_name() == "int");
        REQUIRE(field.get_value_type() == FieldValueType::integer);
        REQUIRE(std::get<std::uintmax_t>(field.get_value()) == value);

        auto opt = field.get_integer();
        REQUIRE(opt.has_value());
        REQUIRE(*opt == value);

        field.visit([&]<typename T>(T&& value) { REQUIRE(std::same_as<std::remove_cvref_t<T>, std::uintmax_t>); });
    }

    SECTION("Test string field") {
        const std::string value = "value";

        Field field{"string", value};
        REQUIRE(field.get_name() == "string");
        REQUIRE(field.get_value_type() == FieldValueType::string);
        REQUIRE(std::get<std::string>(field.get_value()) == value);

        auto opt = field.get_string();
        REQUIRE(opt.has_value());
        REQUIRE(*opt == value);

        field.visit([&]<typename T>(T&& value) { REQUIRE(std::same_as<std::remove_cvref_t<T>, std::string>); });
    }

    SECTION("Test 'field_from_string'") {
        SECTION("Boolean field with 'true' value") {
            const std::string str =
                GENERATE(as<std::string>{}, "value:true", " \tvalue: true", "value\t:\ttrue", "\tvalue\t:\ttrue\t");
            Field field = field_from_string(str);
            REQUIRE(field.get_name() == "value");
            REQUIRE(field.get_value_type() == FieldValueType::boolean);
            REQUIRE(field.get_boolean().value() == true);
        }

        SECTION("Boolean field with 'false' value") {
            const std::string str =
                GENERATE(as<std::string>{}, "value:false", " \tvalue: false", "value\t:\tfalse", "\tvalue\t:\tfalse\t");
            Field field = field_from_string(str);
            REQUIRE(field.get_name() == "value");
            REQUIRE(field.get_value_type() == FieldValueType::boolean);
            REQUIRE(field.get_boolean().value() == false);
        }

        SECTION("Integer field") {
            const std::string str = GENERATE(as<std::string>{}, "int:12", "int  :   12", "\tint\t:   12\t",
                                             "   int\t:   12\t", "   int\t:12", "\tint\t:\t12\t");
            Field field = field_from_string(str);
            REQUIRE(field.get_name() == "int");
            REQUIRE(field.get_value_type() == FieldValueType::integer);
            REQUIRE(field.get_integer().value() == 12);
        }

        SECTION("String field") {
            const std::string str = GENERATE(as<std::string>{}, "str: test", "str: 'test'", "\tstr:  'test'   ",
                                             "\tstr\t:\ttest\t", "\tstr : 'test'\t");
            Field field = field_from_string(str);
            REQUIRE(field.get_name() == "str");
            REQUIRE(field.get_value_type() == FieldValueType::string);
            REQUIRE(field.get_string().value() == "test");
        }

        SECTION("String with extra spaces") {
            Field field = field_from_string("    path  : 'My Documents/Doc with trailing space '    ");
            REQUIRE(field.get_name() == "path");
            REQUIRE(field.get_value_type() == FieldValueType::string);
            REQUIRE(field.get_string().value() == "My Documents/Doc with trailing space ");
        }
    }

    SECTION("Test 'field_from_string' with invalid values") {
        const std::string str =
            GENERATE(as<std::string>{}, "bad", "bad:", "  bad    :    ", "    worse : 18446744073709551617   ",
                     "   inva_liD-NAME  :    hello ", "   bad__ : true   ");
        REQUIRE_THROWS_AS(field_from_string(str), ProtocolError);
    }
}
