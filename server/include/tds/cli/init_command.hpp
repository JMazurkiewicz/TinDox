#pragma once

#include "tds/cli/command_interface.hpp"

#include <filesystem>
#include <ostream>
#include <span>
#include <string_view>

namespace tds::cli {
    class InitCommand : public CommandInterface<InitCommand> {
    public:
        static constexpr std::string_view name = "init";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void create_config_directory();
        void create_default_config();
        void create_default_users();

        void handle_filesystem_error(const std::filesystem::filesystem_error& e);
        void handle_exception(const std::exception& e);

        std::filesystem::path m_location;
    };
}