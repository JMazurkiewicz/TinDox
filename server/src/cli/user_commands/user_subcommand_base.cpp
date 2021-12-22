#include "tds/cli/user_commands/user_subcommand_base.hpp"

#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/user/user_table.hpp"

namespace tds::cli::user_commands {
    void UserSubcommandBase::set_user_table(user::UserTable& user_table) {
        m_user_table_ptr = &user_table;
    }

    user::UserTable& UserSubcommandBase::get_user_table() {
        if(m_user_table_ptr) {
            return *m_user_table_ptr;
        } else {
            throw InvalidCommandExecutionError{"user table is invalid"};
        }
    }

    const user::UserTable& UserSubcommandBase::get_user_table() const {
        if(m_user_table_ptr) {
            return *m_user_table_ptr;
        } else {
            throw InvalidCommandExecutionError{"user table is invalid"};
        }
    }

}
