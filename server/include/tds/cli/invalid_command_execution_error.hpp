#pragma once

#include <stdexcept>

namespace tds::cli {
    class InvalidCommandExecutionError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}
