#pragma once

#include "tds/user/user_table.hpp"

#include <filesystem>
#include <span>
#include <string_view>

namespace tds::cli {
    class UserCommand {
    public:
        static constexpr std::string_view name = "user";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void load_user_table(std::filesystem::path path);

        std::string_view m_subcommand;
        std::span<const std::string_view> m_subcommand_args;
        user::UserTable m_user_table;
    };
}
