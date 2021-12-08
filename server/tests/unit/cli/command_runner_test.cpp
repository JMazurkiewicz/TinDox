#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_interface.hpp"
#include "tds/cli/command_runner.hpp"

using namespace tds::cli;

struct SpawnCommand : CommandInterface<SpawnCommand> {
    static constexpr std::string_view name = "spawn";

    void parse_arguments(std::span<const std::string_view>) { }
    void execute() { }
};

struct PrintCommand : CommandInterface<PrintCommand> {
    static constexpr std::string_view name = "print";

    void parse_arguments(std::span<const std::string_view>) { }
    void execute() {
        set_error();
    }
};

TEST_CASE("tds::cli::CommandRunner", "[cli]") {
    SECTION("Check commands") {
        REQUIRE(Command<SpawnCommand>);
        REQUIRE(Command<PrintCommand>);
    }

    CommandRunner<SpawnCommand, PrintCommand> runner;
    const std::span<std::string_view> args;

    SECTION("Run spwan command") {
        REQUIRE(runner.run("spawn", args) == 0);
    }

    SECTION("Run print command") {
        REQUIRE(runner.run("print", args) == 1);
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
