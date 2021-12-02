#pragma once

#include <stdexcept>

namespace tds::cli {
    class cli_error : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };
}
