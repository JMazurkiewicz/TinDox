
#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/field_value.hpp"
#include "tds/protocol/protocol_interpreter.hpp"

using namespace tds::protocol;
using namespace std::string_view_literals;

TEST_CASE("tds::protocol::ProtocolInterpreter", "[protocol]") {
    SECTION("Regular request with extra input at the end") {
        ProtocolInterpreter interpreter;

        interpreter.add_bytes("auth\nlogin: ad"sv);
        REQUIRE(!interpreter.has_available_requests());

        interpreter.add_bytes("min\npasswd: admin");
        REQUIRE(!interpreter.has_available_requests());

        interpreter.add_bytes("\n\npwd");
        REQUIRE(interpreter.has_available_requests());

        const auto requests = interpreter.get_requests();
        REQUIRE(requests.size() == 1);

        const Request& request = requests.front();
        REQUIRE(request.get_name() == "auth");

        const auto fields = request.get_fields();
        REQUIRE(fields.size() == 2);
        REQUIRE(fields[0].get_value_type() == FieldValueType::string);
        REQUIRE(fields[0].get_name() == "login");
        REQUIRE(fields[0].get_string() == "admin");
        REQUIRE(fields[1].get_value_type() == FieldValueType::string);
        REQUIRE(fields[1].get_name() == "passwd");
        REQUIRE(fields[1].get_string() == "admin");
    }

    SECTION("Special case where newline is first character of input") {
        ProtocolInterpreter interpreter;

        interpreter.add_bytes("auth"sv);
        REQUIRE(!interpreter.has_available_requests());

        interpreter.add_bytes("\nlogin: admin\npasswd: admin\n\n");
        REQUIRE(interpreter.has_available_requests());

        const auto requests = interpreter.get_requests();
        REQUIRE(requests.size() == 1);

        const Request& request = requests.front();
        REQUIRE(request.get_name() == "auth");

        const auto fields = request.get_fields();
        REQUIRE(fields.size() == 2);
        REQUIRE(fields[0].get_value_type() == FieldValueType::string);
        REQUIRE(fields[0].get_name() == "login");
        REQUIRE(fields[0].get_string() == "admin");
        REQUIRE(fields[1].get_value_type() == FieldValueType::string);
        REQUIRE(fields[1].get_name() == "passwd");
        REQUIRE(fields[1].get_string() == "admin");
    }

    SECTION("Every part of input end with newline character") {
        ProtocolInterpreter interpreter;

        interpreter.add_bytes("auth\n"sv);
        REQUIRE(!interpreter.has_available_requests());

        interpreter.add_bytes("login: admin\n");
        REQUIRE(!interpreter.has_available_requests());

        interpreter.add_bytes("passwd: admin\n"sv);
        REQUIRE(!interpreter.has_available_requests());

        interpreter.add_bytes("\n"sv);
        REQUIRE(interpreter.has_available_requests());

        const auto requests = interpreter.get_requests();
        REQUIRE(requests.size() == 1);

        const Request& request = requests.front();
        REQUIRE(request.get_name() == "auth");

        const auto fields = request.get_fields();
        REQUIRE(fields.size() == 2);
        REQUIRE(fields[0].get_value_type() == FieldValueType::string);
        REQUIRE(fields[0].get_name() == "login");
        REQUIRE(fields[0].get_string() == "admin");
        REQUIRE(fields[1].get_value_type() == FieldValueType::string);
        REQUIRE(fields[1].get_name() == "passwd");
        REQUIRE(fields[1].get_string() == "admin");
    }
}
