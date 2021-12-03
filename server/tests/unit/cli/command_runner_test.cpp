#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_runner.hpp"

using namespace tds::cli;

enum class magic_value {
    log = 1,
    init = 2,
    print = 3,
};

magic_value g_observable_value;

struct log_command {
    static std::string_view name() {
        return "log";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) {
        g_observable_value = magic_value::log;
    }
};

struct init_command {
    static std::string_view name() {
        return "init";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) {
        g_observable_value = magic_value::init;
    }
};

struct print_command {
    static std::string_view name() {
        return "print";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) {
        g_observable_value = magic_value::print;
    }
};

TEST_CASE("tds::cli::command_runner", "[cli]") {
    SECTION("Check commands") {
        REQUIRE(command<log_command>);
        REQUIRE(command<init_command>);
        REQUIRE(command<print_command>);
    }

    command_runner<log_command, init_command, print_command> runner;
    const std::span<std::string_view> args;

    SECTION("Run log command") {
        runner.run("log", args);
        REQUIRE(g_observable_value == magic_value::log);
    }

    SECTION("Run init command") {
        runner.run("init", args);
        REQUIRE(g_observable_value == magic_value::init);
    }

    SECTION("Run print command") {
        runner.run("print", args);
        REQUIRE(g_observable_value == magic_value::print);
    }

    SECTION("Run invalid command") {
        REQUIRE_THROWS_AS(runner.run("invalid", args), cli_error);

        try {
            runner.run("invalid", args);
        } catch(const std::exception& e) {
            const std::string expected = "tds: 'invalid' is not a tds command. See 'tds help'.";
            REQUIRE(e.what() == expected);
        }
    }
}
