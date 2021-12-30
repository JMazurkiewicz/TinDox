#pragma once

#include <stdexcept>

namespace tds::command {
    class NoSuchCommandError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}
