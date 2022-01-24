#pragma once

#include "tds/cli/cli_error.hpp"

#include <stdexcept>

namespace tds::cli {
    class InvalidCommandExecutionError : public CliError {
    public:
        using CliError::CliError;
    };
}
