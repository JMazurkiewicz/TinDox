#pragma once

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
        void parse_config_directory_path(std::filesystem::path path);

        std::filesystem::path m_users_file;
        std::string_view m_subcommand;
        std::span<const std::string_view> m_subcommand_args;
    };
}
