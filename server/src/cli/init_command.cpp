#include "tds/cli/init_command.hpp"

#include "tds/config/defaults.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace tds::cli {
    InitCommand::InitCommand()
        : m_exit_status{0} { }

    int InitCommand::do_execute(std::span<const std::string_view> args) {
        set_location(args);
        execute_steps();
        return m_exit_status;
    }

    void InitCommand::set_location(std::span<const std::string_view> args) {
        switch(args.size()) {
        case 0:
            m_location = fs::current_path();
            break;
        case 1:
            m_location = args[0];
            if(!fs::exists(m_location)) {
                std::cerr << "error: location " << m_location << " does not exist\n";
                m_exit_status = 1;
            }
            break;
        default:
            std::cerr << "error: invalid arguments\n"
                         "usage: tds init [<location>]\n";
            m_exit_status = 1;
        }
    }

    void InitCommand::execute_steps() {
        try {
            for(auto op : {&InitCommand::create_config_directory, &InitCommand::create_default_config}) {
                if(m_exit_status != 0) {
                    break;
                }
                (this->*op)();
            }
        } catch(const fs::filesystem_error& e) {
            std::cerr << "error: " << e.what() << "\npath1: " << e.path1() << "\npath2: " << e.path2()
                      << "\ncode: " << e.code() << '\n';
            m_exit_status = 1;
        } catch(const std::exception& e) {
            std::cerr << "error: " << e.what() << '\n';
            m_exit_status = 1;
        }
    }

    void InitCommand::create_config_directory() {
        const fs::path config_name = m_location / ".tds";

        if(fs::exists(config_name)) {
            std::cerr << "error: config already exists in " << m_location << '\n';
            m_exit_status = 1;
        } else {
            fs::create_directory(config_name);
            m_location = std::move(config_name);
        }
    }

    void InitCommand::create_default_config() {
        std::ofstream config_file{m_location / "config"};
        if(!config_file.good()) {
            m_exit_status = 1;
            return;
        }

        config_file << "[config]\n"
                       "max_thread_count = "
                    << config::defaults::get_default_max_thread_count()
                    << "\n"
                       "max_user_count = "
                    << config::defaults::get_default_max_user_count()
                    << "\n"
                       "backlog = "
                    << config::defaults::get_default_backlog() << '\n';
    }
}
