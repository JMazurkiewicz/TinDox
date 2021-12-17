#pragma once

#include "tds/cli/command_interface.hpp"
#include "tds/config/server_config.hpp"

#include <filesystem>
#include <optional>

namespace tds::cli {
    class RunCommand : public CommandInterface<RunCommand> {
    public:
        static constexpr std::string_view name = "run";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void parse_port(std::string_view arg);
        void parse_root(std::string_view arg);

        void make_root_path();
        void read_config();
        void apply_arguments();

        std::optional<std::uint16_t> m_port;
        std::optional<std::filesystem::path> m_root;

        config::ServerConfig m_config;
    };
}
