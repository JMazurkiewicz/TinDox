#pragma once

#include "tds/protocol/execution/auth.hpp"
#include "tds/protocol/execution/bye.hpp"
#include "tds/protocol/execution/cd.hpp"
#include "tds/protocol/execution/command_executor.hpp"
#include "tds/protocol/execution/cp.hpp"
#include "tds/protocol/execution/dl.hpp"
#include "tds/protocol/execution/dls.hpp"
#include "tds/protocol/execution/exit.hpp"
#include "tds/protocol/execution/logout.hpp"
#include "tds/protocol/execution/ls.hpp"
#include "tds/protocol/execution/mkdir.hpp"
#include "tds/protocol/execution/mv.hpp"
#include "tds/protocol/execution/name.hpp"
#include "tds/protocol/execution/perms.hpp"
#include "tds/protocol/execution/pwd.hpp"
#include "tds/protocol/execution/rename.hpp"
#include "tds/protocol/execution/rm.hpp"
#include "tds/protocol/execution/tree.hpp"
#include "tds/protocol/execution/ul.hpp"

namespace tds::protocol {
    using DefaultCommandExecutor =
        execution::CommandExecutor<execution::Auth, execution::Bye, execution::Cd, execution::Cp, execution::Dl,
                                   execution::Dls, execution::Exit, execution::Logout, execution::Ls, execution::Mkdir,
                                   execution::Mv, execution::Name, execution::Perms, execution::Pwd, execution::Rename,
                                   execution::Rm, execution::Tree, execution::Ul>;
}
