#pragma once

#include <stdexcept>

namespace tds::cli {
    class CliError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}
