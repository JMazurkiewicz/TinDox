#pragma once

#include <cstddef>

namespace tds::config::limits {
    static constexpr int max_try_count = 3;
    static constexpr std::size_t max_username_length = 20;
    static constexpr std::size_t max_password_length = 30;
}
