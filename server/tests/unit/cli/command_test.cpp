#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command.hpp"

using namespace tds::cli;

struct good_command1 {
    static std::string_view name() {
        return "good_command1";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) { }
};

struct good_command2 {
    static const char* name() {
        return "good_command2";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) { }
};

struct bad_command1 {
    static int name() {
        return 0;
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) { }
};

struct bad_command2 {
    static const char* name() {
        return "bad_command2";
    }

    void execute([[maybe_unused]] std::span<std::string_view>) { }
};

struct bad_command3 { };

TEST_CASE("tds::cli::command", "[cli]") {
    SECTION("Test good commands") {
        REQUIRE(command<good_command1>);
        REQUIRE(command<good_command2>);
    }

    SECTION("Test bad commands") {
        REQUIRE_FALSE(command<bad_command1>);
        REQUIRE_FALSE(command<bad_command2>);
        REQUIRE_FALSE(command<bad_command3>);
    }
}
