#include <catch2/catch_test_macros.hpp>

#include "tds/cli/command_interface.hpp"
#include "tds/cli/command_runner.hpp"

#include <string_view>

using namespace tds::cli;
using namespace std::string_view_literals;

struct GoodCommand : CommandInterface<GoodCommand> {
    static constexpr std::string_view name = "good";

    void parse_arguments(std::span<const std::string_view> args) {
        m_argc = static_cast<int>(args.size());
    }

    void execute() { }

    std::size_t m_argc = 0;
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

        command.parse_arguments(args);
        REQUIRE(command.m_argc == 3);
    }
}
