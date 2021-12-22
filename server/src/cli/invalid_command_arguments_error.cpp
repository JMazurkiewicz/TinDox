#include "tds/cli/invalid_command_arguments_error.hpp"

namespace tds::cli {
    InvalidCommandArgumentsError::InvalidCommandArgumentsError(const std::string& what, std::string tip)
        : InvalidCommandArgumentsError(what.c_str(), std::move(tip)) { }

    InvalidCommandArgumentsError::InvalidCommandArgumentsError(const char* what, std::string tip)
        : CliError(what)
        , m_tip{std::move(tip)} { }

    std::string_view InvalidCommandArgumentsError::get_tip() const noexcept {
        return m_tip;
    }
}
