#include <catch2/catch_test_macros.hpp>

#include "tds/cli/cli_error.hpp"
#include "tds/cli/command.hpp"
#include "tds/cli/init_command.hpp"
#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"

#include <array>
#include <filesystem>
#include <stdexcept>

using namespace tds::cli;
using namespace std::string_view_literals;
namespace fs = std::filesystem;

TEST_CASE("tds::cli::InitCommand", "[cli]") {
    SECTION("Is 'InitCommand' a valid command?") {
        REQUIRE(Command<InitCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "init";
        REQUIRE(InitCommand::name == expected);
    }

    SECTION("Test creating instance in current path") {
        const fs::path test_dir = "/tmp/tds_test1";
        fs::create_directory(test_dir);
        fs::current_path(test_dir);

        InitCommand init;
        REQUIRE_NOTHROW(init.parse_arguments({}));
        REQUIRE_NOTHROW(init.execute());

        const fs::path tds_root = test_dir / ".tds";
        REQUIRE(fs::exists(tds_root));
        REQUIRE(fs::exists(tds_root / "config"));
        REQUIRE(fs::exists(tds_root / "users"));
    }

    SECTION("Test creating instance in different path") {
        const fs::path test_dir = "/tmp/tds_test2";
        fs::create_directory(test_dir);
        const std::array args = {std::string_view{test_dir.native()}};

        InitCommand init;
        REQUIRE_NOTHROW(init.parse_arguments(args));
        REQUIRE_NOTHROW(init.execute());

        const fs::path tds_root = test_dir / ".tds";
        REQUIRE(fs::exists(tds_root));
        REQUIRE(fs::exists(tds_root / "config"));
        REQUIRE(fs::exists(tds_root / "users"));
    }

    SECTION("Test creating instance where instance already exists") {
        const fs::path test_dir = "/tmp/tds_test2";
        const std::array args = {std::string_view{test_dir.native()}};

        InitCommand init;
        REQUIRE_NOTHROW(init.parse_arguments(args));
        REQUIRE_THROWS_AS(init.execute(), InvalidCommandExecutionError);
    }

    SECTION("Test creating instance in different (invalid) path") {
        const fs::path test_dir = "i/do/not/exist";
        const std::array args = {std::string_view{test_dir.native()}};

        InitCommand init;
        REQUIRE_THROWS_AS(init.parse_arguments(args), InvalidCommandExecutionError);
    }

    SECTION("Test invalid arguments") {
        const std::array args = {"bad"sv, "arguments"sv};
        InitCommand init;
        REQUIRE_THROWS_AS(init.parse_arguments(args), InvalidCommandArgumentsError);
    }
}
