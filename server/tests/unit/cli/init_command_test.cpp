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
        tds::unit::test_in_new_process([] {
            const fs::path test_dir = "/tmp/tds_test1";
            fs::create_directory(test_dir);
            fs::current_path(test_dir);

            InitCommand init;
            const int status = init({});

            const fs::path tds_root = test_dir / ".tds";
            const bool success = status == 0 && fs::exists(tds_root) && fs::exists(tds_root / "config") &&
                                 fs::exists(tds_root / "users");
            return static_cast<int>(!success);
        });
    }

    SECTION("Test creating instance in different path") {
        tds::unit::test_in_new_process([] {
            const fs::path test_dir = "/tmp/tds_test2";
            fs::create_directory(test_dir);
            const auto args = std::to_array<std::string_view>({test_dir.native()});

            InitCommand init;
            const int status = init(args);

            const fs::path tds_root = test_dir / ".tds";
            const bool success = status == 0 && fs::exists(tds_root) && fs::exists(tds_root / "config") &&
                                 fs::exists(tds_root / "users");
            return static_cast<int>(!success);
        });
    }
}
