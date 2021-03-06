#include "tds/cli/user_commands/user_add_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/linux/echoless_terminal_guard.hpp"
#include "tds/user/user_table.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include <fmt/core.h>

namespace tds::cli::user_commands {
    namespace {
        [[noreturn]] void throw_too_many_attempts_error() {
            throw InvalidCommandExecutionError{
                fmt::format("too many attempts (max is {})", UserAddCommand::max_try_count)};
        }
    }

    void UserAddCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() != 0) {
            throw InvalidCommandArgumentsError{"user add command does not take any arguments", "tds user add"};
        }
    }

    void UserAddCommand::execute() {
        read_username();
        check_username_uniqueness();
        read_password();
        read_repeated_password();
        get_user_table().add_user(m_username, m_password);
    }

    void UserAddCommand::read_username() {
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter username: ";
            } else {
                std::cout << "Invalid username - use letters or numbers only. Try again: ";
            }

            std::getline(std::cin, m_username);
            if(user::UserTable::is_username_ok(m_username)) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserAddCommand::check_username_uniqueness() const {
        if(get_user_table().has_user(m_username)) {
            throw InvalidCommandExecutionError{fmt::format("user named '{}' already exists", m_username)};
        }
    }

    void UserAddCommand::read_password() {
        linux::EcholessTerminalGuard terminal_guard;
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter password: ";
            } else {
                std::cout << "Password is too long, try something shorter: ";
            }

            std::getline(std::cin, m_password);
            std::cout << '\n';
            if(user::UserTable::is_password_ok(m_password)) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserAddCommand::read_repeated_password() const {
        std::string repeated_password;
        linux::EcholessTerminalGuard terminal_guard;
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Repeat password: ";
            } else {
                std::cout << "Passwords don't match. Try again: ";
            }

            std::getline(std::cin, repeated_password);
            std::cout << '\n';
            if(repeated_password == m_password) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }
}
