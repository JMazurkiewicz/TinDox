#pragma once

#include "tds/protocol/execution/auth.hpp"
#include "tds/protocol/execution/cd.hpp"
#include "tds/protocol/execution/command_executor.hpp"
#include "tds/protocol/execution/exit.hpp"
#include "tds/protocol/execution/logout.hpp"
#include "tds/protocol/execution/ls.hpp"
#include "tds/protocol/execution/name.hpp"
#include "tds/protocol/execution/pwd.hpp"

namespace tds::protocol {
    using DefaultCommandExecutor =
        execution::CommandExecutor<execution::Auth, execution::Cd, execution::Exit, execution::Logout, execution::Ls,
                                   execution::Name, execution::Pwd>;
}
