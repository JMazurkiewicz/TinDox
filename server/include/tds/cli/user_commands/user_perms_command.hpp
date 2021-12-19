#pragma once

#include "tds/cli/user_commands/user_subcommand_base.hpp"

#include <span>

namespace tds::cli::user_commands {
    class UserPermsCommand : public UserSubcommandBase {
    public:
        static constexpr std::string_view name = "perms";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();
    };
}
