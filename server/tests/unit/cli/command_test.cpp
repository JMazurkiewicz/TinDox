#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"

using namespace tds::cli;

struct GoodCommand1 {
    static std::string_view name() {
        return "good_command1";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) { }
};

struct GoodCommand2 {
    static const char* name() {
        return "good_command2";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) { }
};

struct BadCommand1 {
    static int name() {
        return 0;
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) { }
};

struct BadCommand2 {
    static const char* name() {
        return "bad_command2";
    }

    void execute([[maybe_unused]] std::span<std::string_view>) { }
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
