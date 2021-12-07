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
            fs::current_path("/tmp");
            InitCommand init;
            init.execute({});

            const fs::path tds_root = fs::current_path() / ".tds";
            const bool status =
                fs::exists(tds_root) && fs::exists(tds_root / "config") && fs::exists(tds_root / "users");
            return static_cast<int>(!status);
        });
    }

    SECTION("Test creating instance in different path") {
        tds::unit::test_in_new_process([] {
            const auto args = std::to_array<std::string_view>({"/tmp"});
            InitCommand init;
            init.execute(args);

            const fs::path tds_root = "/tmp/.tds";
            const bool status =
                fs::exists(tds_root) && fs::exists(tds_root / "config") && fs::exists(tds_root / "users");
            return static_cast<int>(!status);
        });
    }
}
