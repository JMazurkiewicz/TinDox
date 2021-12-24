#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/field.hpp"

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
}
