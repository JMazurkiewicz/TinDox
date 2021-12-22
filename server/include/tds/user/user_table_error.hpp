#pragma once

#include <stdexcept>

namespace tds::user {
    class UserTableError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}
