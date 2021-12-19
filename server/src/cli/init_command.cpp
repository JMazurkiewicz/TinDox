#include "tds/cli/init_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/config/defaults.hpp"
#include "tds/user/permissions.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::cli {
    void InitCommand::parse_arguments(std::span<const std::string_view> args) {
        if(const auto size = args.size(); size == 0) {
            m_location = fs::current_path();
        } else if(size == 1) {
            m_location = args[0];
            if(!fs::exists(m_location)) {
                throw InvalidCommandExecutionError{fmt::format("location {} does not exist", m_location)};
            }
        } else {
            throw InvalidCommandArgumentsError{"invalid argument", "tds init [<location>]"};
        }
    }

    void InitCommand::execute() {
        create_config_directory();
        create_default_config();
        create_default_users();
    }

    void InitCommand::create_config_directory() {
        fs::path config_name = fs::canonical(m_location) / ".tds";

        if(fs::exists(config_name)) {
            throw InvalidCommandExecutionError{fmt::format("tds config already exists in {}", m_location)};
        } else {
            fs::create_directory(config_name);
            m_location = std::move(config_name);
        }
    }

    void InitCommand::create_default_config() {
        const fs::path file_name = m_location / "config";
        std::ofstream config_file{file_name};
        if(!config_file.good()) {
            throw InvalidCommandExecutionError{fmt::format("failed to create config file ({})", file_name)};
        } else {
            config_file << config::defaults::get_default_config_file();
        }
    }

    void InitCommand::create_default_users() {
        const fs::path file_name = m_location / "users";
        std::ofstream users_file{file_name};
        if(!users_file.good()) {
            throw InvalidCommandExecutionError{fmt::format("failed to create users file ({})", file_name)};
        } else {
            users_file << config::defaults::get_default_users_file();
        }
    }
}
