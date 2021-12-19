#include "tds/cli/init_command.hpp"

#include "tds/config/defaults.hpp"
#include "tds/linux/hash.hpp"
#include "tds/user/permissions.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace tds::cli {
    void InitCommand::parse_arguments(std::span<const std::string_view> args) {
        switch(args.size()) {
        case 0:
            m_location = fs::current_path();
            break;

        case 1:
            m_location = args[0];
            if(!fs::exists(m_location)) {
                log_error() << "error: location " << m_location << " does not exist\n";
            }
            break;

        default:
            log_error() << "error: invalid arguments\n"
                           "usage: tds init [<location>]\n";
            break;
        }
    }

    void InitCommand::execute() {
        try {
            execute_steps(&InitCommand::create_config_directory, &InitCommand::create_default_config,
                          &InitCommand::create_default_users);
        } catch(const fs::filesystem_error& e) {
            handle_filesystem_error(e);
        } catch(const std::exception& e) {
            handle_exception(e);
        }
    }

    void InitCommand::create_config_directory() {
        const fs::path config_name = m_location / ".tds";

        if(fs::exists(config_name)) {
            log_error() << "error: config already exists in " << m_location << '\n';
        } else {
            fs::create_directory(config_name);
            m_location = std::move(config_name);
        }
    }

    void InitCommand::create_default_config() {
        const fs::path file_name = m_location / "config";
        std::ofstream config_file{file_name};
        if(!config_file.good()) {
            log_error() << "error: failed to create config file (" << file_name << ")\n";
        } else {
            config_file << "[server]\n"
                           "max_thread_count = "
                        << config::defaults::get_default_max_thread_count()
                        << "\n"
                           "max_user_count = "
                        << config::defaults::get_default_max_user_count()
                        << "\n"
                           "backlog = "
                        << config::defaults::get_default_backlog()
                        << "\n"
                           "port = "
                        << config::defaults::get_default_port() << '\n';
        }
    }

    void InitCommand::create_default_users() {
        const fs::path file_name = m_location / "users";
        std::ofstream users_file{file_name};
        if(!users_file.good()) {
            log_error() << "error: failed to create users file (" << file_name << ")\n";
        } else {
            users_file << "admin:" << linux::hash("admin") << ':' << user::Permissions::all << '\n';
        }
    }

    void InitCommand::handle_filesystem_error(const fs::filesystem_error& e) {
        log_error() << "error: " << e.what() << "\npath1: " << e.path1() << "\npath2: " << e.path2()
                    << "\ncode: " << e.code() << '\n';
    }

    void InitCommand::handle_exception(const std::exception& e) {
        log_error() << "error: " << e.what() << '\n';
    }
}
