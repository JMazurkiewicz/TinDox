#pragma once

#include "tds/protocol/execution/auth.hpp"
#include "tds/protocol/execution/cd.hpp"
#include "tds/protocol/execution/command_executor.hpp"
#include "tds/protocol/execution/ls.hpp"
#include "tds/protocol/execution/pwd.hpp"
#include "tds/protocol/execution/name.hpp"

namespace tds::protocol {
    using DefaultCommandExecutor =
        execution::CommandExecutor<execution::Auth, execution::Cd, execution::Ls, execution::Name, execution::Pwd>;
}
