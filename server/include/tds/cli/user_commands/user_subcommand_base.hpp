#pragma once

#include "tds/user/user_table.hpp"

namespace tds::cli::user_commands {
    class UserSubcommandBase {
    public:
        static constexpr int max_try_count = 3;

        void set_user_table(user::UserTable& user_table);

    protected:
        user::UserTable& get_user_table();
        const user::UserTable& get_user_table() const;

    private:
        user::UserTable* m_user_table_ptr = nullptr;
    };
}
