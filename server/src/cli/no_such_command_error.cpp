#include "tds/cli/no_such_command_error.hpp"

#include <fmt/format.h>

namespace tds::cli {
    NoSuchCommandError::NoSuchCommandError(std::string_view wrong_command)
        : CliError(fmt::format("tds: '{}' is not a tds command. See 'tds help'.", wrong_command)) { }

}
