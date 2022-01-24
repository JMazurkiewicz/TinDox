#include "tds/cli/user_commands/user_passwd_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/linux/echoless_terminal_guard.hpp"
#include "tds/linux/hash.hpp"
#include "tds/user/user_table.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <fmt/format.h>

namespace tds::cli::user_commands {
    namespace {
        [[noreturn]] void throw_too_many_attempts_error() {
            throw InvalidCommandExecutionError{
                fmt::format("too many attempts (max is {})", UserPasswdCommand::max_try_count)};
        }
    }

    void UserPasswdCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() != 1) {
            throw InvalidCommandArgumentsError{"user passwd takes username as argument", "tds user passwd <username>"};
        } else {
            m_username = args[0];
        }
    }

    void UserPasswdCommand::execute() {
        m_old_password_hash = get_user_table().get_password_hash_of_user(m_username);

        read_old_password();
        read_new_password();
        read_new_password_repeated();

        get_user_table().set_password_of_user(m_username, m_new_password);
    }

    void UserPasswdCommand::read_old_password() const {
        linux::EcholessTerminalGuard terminal_guard;
        std::string old_password;
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter old password: ";
            } else {
                std::cout << "Wrong password, try again: ";
            }

            std::getline(std::cin, old_password);
            std::cout << '\n';
            if(get_user_table().verify_password_of_user(m_username, old_password)) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserPasswdCommand::read_new_password() {
        linux::EcholessTerminalGuard terminal_guard;
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter new password: ";
            } else {
                std::cout << "New password is too long, try something shorter: ";
            }

            std::getline(std::cin, m_new_password);
            std::cout << '\n';
            if(user::UserTable::is_password_ok(m_new_password)) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserPasswdCommand::read_new_password_repeated() const {
        linux::EcholessTerminalGuard terminal_guard;
        std::string repeated_password;
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Repeat new password: ";
            } else {
                std::cout << "Passwords don't match. Try again: ";
            }

            std::getline(std::cin, repeated_password);
            std::cout << '\n';
            if(repeated_password == m_new_password) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }
}
