#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/field_value.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/request.hpp"

#include <algorithm>

using namespace tds::protocol;
using namespace std::string_view_literals;

TEST_CASE("tds::protocol::ProtocolInterpreter", "[protocol]") {
    SECTION("Regular request with extra input at the end") {
        ProtocolInterpreter interpreter;

        auto unread = interpreter.commit_bytes("auth\nlogin: ad"sv);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(unread.empty());

        unread = interpreter.commit_bytes("min\npasswd: admin"sv);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(unread.empty());

        unread = interpreter.commit_bytes("\n\npwd"sv);
        REQUIRE(interpreter.has_available_request());
        REQUIRE(std::ranges::equal(unread, "pwd"sv));

        const Request request = interpreter.get_request();
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

        auto unread = interpreter.commit_bytes("auth"sv);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(unread.empty());

        unread = interpreter.commit_bytes("\nlogin: admin\npasswd: admin\n\n"sv);
        REQUIRE(interpreter.has_available_request());
        REQUIRE(unread.empty());

        const Request request = interpreter.get_request();
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

        auto unread = interpreter.commit_bytes("auth\n"sv);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(unread.empty());

        unread = interpreter.commit_bytes("login: admin\n"sv);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(unread.empty());

        unread = interpreter.commit_bytes("passwd: admin\n"sv);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(unread.empty());

        unread = interpreter.commit_bytes("\n"sv);
        REQUIRE(interpreter.has_available_request());
        REQUIRE(unread.empty());

        const Request request = interpreter.get_request();
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

    SECTION("Two commands") {
        ProtocolInterpreter interpreter;

        auto unread = interpreter.commit_bytes("ls\npath: /\n\n"sv);
        REQUIRE(unread.empty());
        REQUIRE(interpreter.has_available_request());

        const Request first_request = interpreter.get_request();
        REQUIRE(first_request.get_name() == "ls");

        const auto first_fields = first_request.get_fields();
        REQUIRE(first_fields.size() == 1);
        REQUIRE(first_fields[0].get_name() == "path");
        REQUIRE(first_fields[0].get_string() == "/");

        unread = interpreter.commit_bytes("cd\npath: /home\n\n"sv);
        REQUIRE(unread.empty());
        REQUIRE(interpreter.has_available_request());

        const Request second_request = interpreter.get_request();
        REQUIRE(second_request.get_name() == "cd");

        const auto second_fields = second_request.get_fields();
        REQUIRE(second_fields.size() == 1);
        REQUIRE(second_fields[0].get_name() == "path");
        REQUIRE(second_fields[0].get_string() == "/home");
    }

    SECTION("Test ignoring empty requests") {
        ProtocolInterpreter interpreter;
        const auto unread = interpreter.commit_bytes("\n\n\n\n\n");
        REQUIRE(unread.empty());
        REQUIRE(!interpreter.has_available_request());
    }

    SECTION("Test two full commands") {
        ProtocolInterpreter interpreter;
        auto unread = interpreter.commit_bytes("auth\nlogin:admin\npasswd:admin\n\n"sv);
        INFO("UNREAD BYTES: \"" << (std::string_view{unread.data(), unread.size()}) << '"');
        REQUIRE(unread.empty());
        REQUIRE(interpreter.has_available_request());
        Request request = interpreter.get_request();
        REQUIRE(request.get_name() == "auth");
        REQUIRE(request.get_fields().size() == 2);

        unread = interpreter.commit_bytes("exit\n\n"sv);
        REQUIRE(unread.empty());
        REQUIRE(interpreter.has_available_request());
        request = interpreter.get_request();
        REQUIRE(request.get_name() == "exit");
        REQUIRE(request.get_fields().empty());
    }
}
