#include "tds/config/defaults.hpp"

#include <thread>

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
}
