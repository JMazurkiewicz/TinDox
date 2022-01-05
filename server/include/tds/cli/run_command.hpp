#pragma once

#include "tds/config/server_config.hpp"

#include <filesystem>
#include <optional>

namespace tds::cli {
    class RunCommand {
    public:
        static constexpr std::string_view name = "run";

        RunCommand();
        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void parse_port(std::string_view arg);
        void parse_config_directory_path(std::string_view arg);

        void prepare_config_directory();
        void read_config();
        void overwrite_port_if_possible();
        void launch_server();

        std::optional<std::uint16_t> m_port;
        std::optional<std::filesystem::path> m_config_directory;
        bool m_debug_mode : 1;

        config::ServerConfig m_config;
    };
}
