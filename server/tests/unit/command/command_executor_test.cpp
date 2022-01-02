#include <catch2/catch_test_macros.hpp>

#include "tds/command/command_executor.hpp"
#include "tds/command/no_such_command_error.hpp"

using namespace tds::command;

struct SpawnCommand {
    static constexpr std::string_view name = "spawn";
    int i = 0;

    void execute() {
        i = 1;
    }
};

struct PrintCommand {
    static constexpr std::string_view name = "print";
    long i = 0;

    void execute() {
        i = 2;
    }
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
        REQUIRE_NOTHROW(executor.execute());
        executor.visit_command([](auto& command) { REQUIRE(command.i == 1); });
    }

    SECTION("Run print command") {
        executor.set_command("print");
        REQUIRE_NOTHROW(executor.execute());
        executor.visit_command([](auto& command) { REQUIRE(command.i == 2); });
    }

    SECTION("Run invalid command") {
        REQUIRE_THROWS_AS(executor.set_command("invalid"), NoSuchCommandError);
    }
}
