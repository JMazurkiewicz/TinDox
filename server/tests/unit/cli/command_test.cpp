#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/command_interface.hpp"

using namespace tds::cli;

struct GoodCommand1 : public CommandInterface<GoodCommand1> {
    static constexpr std::string_view name = "good_command1";

    void parse_arguments(std::span<const std::string_view>) { }
    void execute() { }
};

struct GoodCommand2 : public CommandInterface<GoodCommand2> {
    static constexpr const char* name = "good_command2";

    void parse_arguments(std::span<const std::string_view>) { }
    void execute() { }
};

struct BadCommand1 : public CommandInterface<BadCommand1> {
    static std::string name;

    void parse_arguments(std::span<std::string_view>) { }
    void execute() { }
};

std::string BadCommand1::name = "bad_command1";

struct BadCommand2 : public CommandInterface<BadCommand2> {
    static constexpr std::string_view name = "bad_command2";

    void execute() { }
};

struct BadCommand3 { };

TEST_CASE("tds::cli::Command", "[cli]") {
    SECTION("Test good commands") {
        REQUIRE(Command<GoodCommand1>);
        REQUIRE(Command<GoodCommand2>);
    }

    SECTION("Test bad commands") {
        REQUIRE_FALSE(Command<BadCommand1>);
        REQUIRE_FALSE(Command<BadCommand2>);
        REQUIRE_FALSE(Command<BadCommand3>);
    }
}
