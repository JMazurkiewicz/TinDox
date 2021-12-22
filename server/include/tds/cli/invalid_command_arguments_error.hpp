#pragma once

#include "tds/cli/cli_error.hpp"

namespace tds::cli {
    class InvalidCommandArgumentsError : public CliError {
    public:
        explicit InvalidCommandArgumentsError(const std::string& what, std::string tip);
        explicit InvalidCommandArgumentsError(const char* what, std::string tip);

        std::string_view get_tip() const noexcept;

    private:
        std::string m_tip;
    };
}
