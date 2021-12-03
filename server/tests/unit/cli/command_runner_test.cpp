#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_runner.hpp"

using namespace tds::cli;

enum class MagicValue {
    log = 1,
    init = 2,
    print = 3,
};

MagicValue g_observable_value;

struct LogCommand {
    static std::string_view name() {
        return "log";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) {
        g_observable_value = MagicValue::log;
    }
};

struct InitCommand {
    static std::string_view name() {
        return "init";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) {
        g_observable_value = MagicValue::init;
    }
};

struct PrintCommand {
    static std::string_view name() {
        return "print";
    }

    void execute([[maybe_unused]] std::span<const std::string_view>) {
        g_observable_value = MagicValue::print;
    }
};

TEST_CASE("tds::cli::CommandRunner", "[cli]") {
    SECTION("Check commands") {
        REQUIRE(Command<LogCommand>);
        REQUIRE(Command<InitCommand>);
        REQUIRE(Command<PrintCommand>);
    }

    CommandRunner<LogCommand, InitCommand, PrintCommand> runner;
    const std::span<std::string_view> args;

    SECTION("Run log command") {
        runner.run("log", args);
        REQUIRE(g_observable_value == MagicValue::log);
    }

    SECTION("Run init command") {
        runner.run("init", args);
        REQUIRE(g_observable_value == MagicValue::init);
    }

    SECTION("Run print command") {
        runner.run("print", args);
        REQUIRE(g_observable_value == MagicValue::print);
    }

    SECTION("Run invalid command") {
        REQUIRE_THROWS_AS(runner.run("invalid", args), CliError);

        try {
            runner.run("invalid", args);
        } catch(const std::exception& e) {
            const std::string expected = "tds: 'invalid' is not a tds command. See 'tds help'.";
            REQUIRE(e.what() == expected);
        }
    }
}
