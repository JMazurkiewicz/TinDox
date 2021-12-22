#pragma once

#include <filesystem>
#include <ostream>
#include <span>
#include <string_view>

namespace tds::cli {
    class InitCommand {
    public:
        static constexpr std::string_view name = "init";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void create_config_directory();
        void create_default_config();
        void create_default_users();

        std::filesystem::path m_location;
    };
}
