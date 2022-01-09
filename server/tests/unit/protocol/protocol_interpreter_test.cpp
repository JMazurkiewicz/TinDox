#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/field_value.hpp"
#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/request.hpp"

#include <algorithm>

using namespace tds::protocol;
using namespace std::string_view_literals;

TEST_CASE("tds::protocol::ProtocolInterpreter", "[protocol]") {
    SECTION("Regular request with extra input at the end") {
        ProtocolInterpreter interpreter;

        std::span<const char> input = "auth\nlogin: ad"sv;
        interpreter.commit_bytes(input);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(input.empty());

        input = "min\npasswd: admin"sv;
        interpreter.commit_bytes(input);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(input.empty());

        input = "\n\npwd"sv;
        interpreter.commit_bytes(input);
        REQUIRE(interpreter.has_available_request());
        REQUIRE(std::ranges::equal(input, "pwd"sv));

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

        std::span<const char> input = "auth"sv;
        interpreter.commit_bytes(input);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(input.empty());

        input = "\nlogin: admin\npasswd: admin\n\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(interpreter.has_available_request());
        REQUIRE(input.empty());

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

        std::span<const char> input = "auth\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(input.empty());

        input = "login: admin\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(input.empty());

        input = "passwd: admin\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(!interpreter.has_available_request());
        REQUIRE(input.empty());

        input = "\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(interpreter.has_available_request());
        REQUIRE(input.empty());

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

        std::span<const char> input = "ls\npath: /\n\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(input.empty());
        REQUIRE(interpreter.has_available_request());

        const Request first_request = interpreter.get_request();
        REQUIRE(first_request.get_name() == "ls");

        const auto first_fields = first_request.get_fields();
        REQUIRE(first_fields.size() == 1);
        REQUIRE(first_fields[0].get_name() == "path");
        REQUIRE(first_fields[0].get_string() == "/");

        input = "cd\npath: /home\n\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(input.empty());
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
        std::span<const char> input = "\n\n\n\n\n";
        interpreter.commit_bytes(input);
        REQUIRE(input.empty());
        REQUIRE(!interpreter.has_available_request());
    }

    SECTION("Test two full commands") {
        ProtocolInterpreter interpreter;
        std::span<const char> input = "auth\nlogin:admin\npasswd:admin\n\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(input.empty());
        REQUIRE(interpreter.has_available_request());
        Request request = interpreter.get_request();
        REQUIRE(request.get_name() == "auth");
        REQUIRE(request.get_fields().size() == 2);

        input = "exit\n\n"sv;
        interpreter.commit_bytes(input);
        REQUIRE(input.empty());
        REQUIRE(interpreter.has_available_request());
        request = interpreter.get_request();
        REQUIRE(request.get_name() == "exit");
        REQUIRE(request.get_fields().empty());
    }

    SECTION("Test bad requests") {
        ProtocolInterpreter interpreter;
        std::span<const char> input = "ls\nls\n\n"sv;
        REQUIRE_THROWS_AS(interpreter.commit_bytes(input), ProtocolError);
        REQUIRE(input.empty());
    }

    SECTION("Test three bad requests") {
        ProtocolInterpreter interpreter;
        std::span<const char> input = {
            "ls\nls\nls\n\n"
            "pwd\npwd:pwd\nllllll\n\n"
            "aaa\nndssd\nfdfd\n;;;;;;;\n\n"sv
        };

        int loop_counter = 0;

        do {
            REQUIRE_THROWS_AS(interpreter.commit_bytes(input), ProtocolError);
            ++loop_counter;
        } while(!input.empty());

        REQUIRE(loop_counter == 3);
    }
}
