#pragma once

#include "tds/cli/cli_error.hpp"

#include <string_view>

namespace tds::cli {
    class NoSuchCommandError : public CliError {
    public:
        explicit NoSuchCommandError(std::string_view wrong_command);
    };
}
