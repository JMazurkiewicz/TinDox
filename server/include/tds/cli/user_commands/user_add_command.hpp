#pragma once

#include "tds/cli/user_commands/user_subcommand_base.hpp"

#include <span>

namespace tds::cli::user_commands {
    class UserAddCommand : public UserSubcommandBase {
    public:
        static constexpr std::string_view name = "add";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void read_username();
        void check_username_uniqueness() const;
        void read_password();
        void read_repeated_password() const;
        void write_new_user() const;

        bool is_username_valid() const;
        bool is_password_valid() const;

        std::string m_username;
        std::string m_password;
    };
}
