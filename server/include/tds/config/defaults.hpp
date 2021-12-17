#pragma once

#include "tds/ip/port.hpp"

namespace tds::config::defaults {
    int get_default_max_thread_count();
    int get_default_max_user_count();
    int get_default_backlog();
    ip::Port get_default_port();
}
