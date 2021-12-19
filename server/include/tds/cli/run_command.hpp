#pragma once

#include "tds/config/server_config.hpp"

#include <filesystem>
#include <optional>

namespace tds::cli {
    class RunCommand {
    public:
        static constexpr std::string_view name = "run";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void parse_port(std::string_view arg);
        void parse_config_directory_path(std::string_view arg);

        void make_config_directory_path();
        void read_config();
        void overwrite_config_with_cli_arguments();
        void launch_server();

        std::optional<std::uint16_t> m_port;
        std::optional<std::filesystem::path> m_config_directory;

        config::ServerConfig m_config;
    };
}
