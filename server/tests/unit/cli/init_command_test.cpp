#include <catch2/catch_test_macros.hpp>
#include <unit.hpp>

#include "tds/cli/command.hpp"
#include "tds/cli/init_command.hpp"

#include <array>
#include <filesystem>

using namespace tds::cli;
using namespace std::string_view_literals;
namespace fs = std::filesystem;

TEST_CASE("tds::cli::InitCommand", "[cli]") {
    SECTION("Is `InitCommand` a valid command?") {
        REQUIRE(Command<InitCommand>);
    }

    SECTION("Check name") {
        const std::string_view expected = "init";
        REQUIRE(InitCommand::name == expected);
        REQUIRE(InitCommand::get_name() == expected);
    }

    SECTION("Test creating instance in current path") {
        const fs::path test_dir = "/tmp/tds_test1";
        fs::create_directory(test_dir);
        fs::current_path(test_dir);

        InitCommand init;
        const int status = init({});

        const fs::path tds_root = test_dir / ".tds";
        REQUIRE(status == 0);
        REQUIRE(fs::exists(tds_root));
        REQUIRE(fs::exists(tds_root / "config"));
        REQUIRE(fs::exists(tds_root / "users"));
    }

    SECTION("Test creating instance in different path") {
        const fs::path test_dir = "/tmp/tds_test2";
        fs::create_directory(test_dir);
        const auto args = std::to_array<std::string_view>({test_dir.native()});

        InitCommand init;
        const int status = init(args);

        const fs::path tds_root = test_dir / ".tds";
        REQUIRE(status == 0);
        REQUIRE(fs::exists(tds_root));
        REQUIRE(fs::exists(tds_root / "config"));
        REQUIRE(fs::exists(tds_root / "users"));
    }

    SECTION("Test creating instance in different (invalid) path") {
        const fs::path test_dir = "i/do/not/exist";
        const auto args = std::to_array<std::string_view>({test_dir.native()});

        InitCommand init;
        REQUIRE(init(args) != 0);
    }

    SECTION("Test invalid arguments") {
        const auto args = std::to_array<std::string_view>({"bad", "arguments"});
        InitCommand init;
        REQUIRE(init(args) != 0);
    }
}
