#pragma once

#include "tds/user/user_table.hpp"

namespace tds::cli::user_commands {
    class UserSubcommandBase {
    public:
        void set_user_table(user::UserTable& user_table);

    protected:
        user::UserTable& get_user_table();
        const user::UserTable& get_user_table() const;

    private:
        user::UserTable* m_user_table_ptr = nullptr;
    };
}
