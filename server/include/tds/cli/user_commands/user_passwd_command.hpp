#pragma once

#include "tds/cli/user_commands/user_subcommand_base.hpp"

#include <span>

namespace tds::cli::user_commands {
    class UserPasswdCommand : public UserSubcommandBase {
    public:
        static constexpr std::string_view name = "passwd";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void read_old_password() const;
        void read_new_password();
        void read_new_password_repeated() const;

        std::string m_username;
        std::string m_old_password_hash;
        std::string m_new_password;
    };
}
