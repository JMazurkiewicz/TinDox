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
            return static_cast<int>(!fs::exists(fs::current_path() / ".tds"));
        });
    }

    SECTION("Test creating instance in different path") {
        tds::unit::test_in_new_process([] {
            InitCommand init;
            const auto args = std::to_array<std::string_view>({"/tmp"});
            init.execute(args);
            return static_cast<int>(!fs::exists("/tmp/.tds"));
        });
    }
}
