#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_interface.hpp"
#include "tds/cli/command_runner.hpp"

using namespace tds::cli;

struct LogCommand : CommandInterface<LogCommand> {
    static constexpr std::string_view name = "log";

    int do_execute([[maybe_unused]] std::span<const std::string_view>) {
        return 0xAA;
    }
};

struct InitCommand : CommandInterface<InitCommand> {
    static constexpr std::string_view name = "init";

    int do_execute([[maybe_unused]] std::span<const std::string_view>) {
        return 0xBB;
    }
};

struct PrintCommand : CommandInterface<PrintCommand> {
    static constexpr std::string_view name = "print";

    int do_execute([[maybe_unused]] std::span<const std::string_view>) {
        return 0xCC;
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
        REQUIRE(runner.run("log", args) == 0xAA);
    }

    SECTION("Run init command") {
        REQUIRE(runner.run("init", args) == 0xBB);
    }

    SECTION("Run print command") {
        REQUIRE(runner.run("print", args) == 0xCC);
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
