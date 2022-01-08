#pragma once

#include "tds/ip/port.hpp"

#include <string>

namespace tds::config::defaults {
    int get_default_max_thread_count() noexcept;
    int get_default_max_user_count() noexcept;
    int get_default_backlog() noexcept;
    ip::Port get_default_port() noexcept;
    std::uintmax_t get_default_upload_max_size() noexcept;

    std::string get_default_config_file();
    std::string get_default_users_file();
}
