#include <catch2/catch_test_macros.hpp>

#include "tds/command/command.hpp"

using namespace tds::command;

struct GoodCommand1 {
    static constexpr std::string_view name = "good_command1";
    void execute() { }
};

struct GoodCommand2 {
    static constexpr const char* name = "good_command2";
    void execute() { }
};

struct BadCommand1 {
    static std::string name;
    void execute() { }
};

std::string BadCommand1::name = "bad_command1";

struct BadCommand2 {
    static constexpr std::string_view name = "bad_command2";

    int execute() {
        return 1;
    }
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
