#include "tds/config/defaults.hpp"

#include "tds/linux/hash.hpp"
#include "tds/user/permissions.hpp"

#include <sstream>
#include <thread>

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace tds::config::defaults {
    int get_default_max_thread_count() noexcept {
        return std::thread::hardware_concurrency();
    }

    int get_default_max_user_count() noexcept {
        return 5 * get_default_max_thread_count();
    }

    int get_default_backlog() noexcept {
        return 32;
    }

    ip::Port get_default_port() noexcept {
        return ip::Port{37666};
    }

    std::uintmax_t get_default_upload_max_size() noexcept {
        return static_cast<std::uintmax_t>(1) << 31;
    }

    std::string get_default_config_file() {
        std::ostringstream string_builder;
        string_builder << "[server]\nmax_thread_count = " << defaults::get_default_max_thread_count();
        string_builder << "\nmax_user_count = " << defaults::get_default_max_user_count();
        string_builder << "\nbacklog = " << defaults::get_default_backlog();
        string_builder << "\nport = " << defaults::get_default_port();
        string_builder << "\nupload_max_size = " << defaults::get_default_upload_max_size() << '\n';
        return std::move(string_builder.str());
    }

    std::string get_default_users_file() {
        std::ostringstream string_builder;
        string_builder << "[[users]]\n";
        string_builder << "name = \"admin\"\n";
        string_builder << "passwd = \"" << linux::hash("admin");
        string_builder << "\"\nperms = \"" << user::Permissions::all << "\"\n";
        return std::move(string_builder.str());
    }
}
