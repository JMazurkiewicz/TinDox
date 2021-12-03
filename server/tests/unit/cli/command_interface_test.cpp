#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_interface.hpp"
#include "tds/cli/command_runner.hpp"

#include <string_view>

using namespace tds::cli;
using namespace std::string_view_literals;

struct GoodCommand : CommandInterface<GoodCommand> {
    static constexpr std::string_view name = "good";

    int do_execute(std::span<const std::string_view> args) {
        return static_cast<int>(args.size());
    }
};

TEST_CASE("tds::cli::CommandInterace<T>", "[cli]") {
    SECTION("Is `GoodCommand` valid command?") {
        REQUIRE(Command<GoodCommand>);
    }

    SECTION("Check `get_name` function") {
        REQUIRE(GoodCommand::get_name() == "good");
    }

    SECTION("Check `execute` function") {
        GoodCommand command;
        std::array args = {"a"sv, "b"sv, "c"sv};
        REQUIRE(command.execute(args) == 3);
    }
}
