#include "tds/config/defaults.hpp"

#include "tds/linux/hash.hpp"
#include "tds/user/permissions.hpp"

#include <sstream>
#include <thread>

#include <fmt/format.h>

namespace tds::config::defaults {
    int get_default_max_thread_count() {
        return std::thread::hardware_concurrency();
    }

    int get_default_max_user_count() {
        return 5 * get_default_max_thread_count();
    }

    int get_default_backlog() {
        return 32;
    }

    ip::Port get_default_port() {
        return ip::Port{37666};
    }

    std::string get_default_config_file() {
        std::ostringstream string_builder;
        string_builder << "[server]\nmax_thread_count = " << config::defaults::get_default_max_thread_count();
        string_builder << "\nmax_user_count = " << config::defaults::get_default_max_user_count();
        string_builder << "\nbacklog = " << config::defaults::get_default_backlog();
        string_builder << "\nport = " << config::defaults::get_default_port() << '\n';
        return string_builder.str();
    }

    std::string get_default_users_file() {
        return fmt::format("admin:{}:{}\n", linux::hash("admin"), user::Permissions::all);
    }
}
