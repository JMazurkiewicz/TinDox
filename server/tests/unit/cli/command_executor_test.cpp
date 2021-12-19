#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_executor.hpp"
#include "tds/cli/no_such_command_error.hpp"

using namespace tds::cli;

struct SpawnCommand {
    static constexpr std::string_view name = "spawn";
    void parse_arguments(std::span<const std::string_view>) { }

    void execute() { }
};

struct PrintCommand {
    static constexpr std::string_view name = "print";
    void parse_arguments(std::span<const std::string_view>) { }

    void execute() { }
};

TEST_CASE("tds::cli::CommandExecutor", "[cli]") {
    SECTION("Check commands") {
        REQUIRE(Command<SpawnCommand>);
        REQUIRE(Command<PrintCommand>);
    }

    CommandExecutor<SpawnCommand, PrintCommand> executor;
    const std::span<std::string_view> args;

    SECTION("Run spawn command") {
        executor.set_command("spawn");
        executor.parse_arguments(args);
        REQUIRE_NOTHROW(executor.execute());
    }

    SECTION("Run print command") {
        executor.set_command("print");
        executor.parse_arguments(args);
        REQUIRE_NOTHROW(executor.execute());
    }

    SECTION("Run invalid command") {
        REQUIRE_THROWS_AS(executor.set_command("invalid"), NoSuchCommandError);

        try {
            executor.set_command("invalid");
        } catch(const std::exception& e) {
            const std::string expected = "tds: 'invalid' is not a tds command. See 'tds help'.";
            REQUIRE(e.what() == expected);
        }
    }
}
