#include "tds/cli/user_commands/user_add_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/linux/hash.hpp"
#include "tds/linux/terminal.hpp"
#include "tds/user/permissions.hpp"
#include "tds/user/user_record_data.hpp"

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
        write_new_user();
    }

    void UserAddCommand::read_username() {
        for(int i = 0; i < max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter username: ";
            } else {
                std::cout << "Invalid username - use letters or numbers only. Try again: ";
            }

            std::getline(std::cin, username);
            if(is_username_valid()) {
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserAddCommand::check_username_uniqueness() const {
        std::ifstream users_file{get_user_file_path()};
        if(!users_file.is_open()) {
            throw InvalidCommandExecutionError{"program failed to open 'users' configuration file"};
        }

        for(std::string line; std::getline(users_file, line);) {
            const auto record = user::make_user_record_data(line);
            if(record.get_username() == username) {
                throw InvalidCommandExecutionError{fmt::format("user named '{}' already exists", username)};
            }
        }
    }

    void UserAddCommand::read_password() {
        linux::Terminal::set_stdin_echo(false);
        std::cout << "Enter password: ";
        std::getline(std::cin, password);

        std::string repeated_password;
        for(int i = 0; i < max_try_count; ++i) {
            std::cout << '\n';
            if(i == 0) {
                std::cout << "Repeat password: ";
            } else {
                std::cout << "Passwords don't match. Try again: ";
            }

            std::getline(std::cin, repeated_password);
            if(repeated_password == password) {
                linux::Terminal::set_stdin_echo(true);
                return;
            }
        }

        std::cout << '\n';
        throw_too_many_attempts_error();
    }

    void UserAddCommand::write_new_user() const {
        std::ofstream users_file{get_user_file_path(), std::ios_base::app};
        users_file << username << ':' << linux::hash(password) << ':'
                   << (user::Permissions::travel | user::Permissions::download | user::Permissions::upload) << '\n';
    }

    bool UserAddCommand::is_username_valid() const {
        const auto test_char = [](unsigned char symbol) {
            return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z') ||
                   ('0' <= symbol && symbol <= '9');
        };
        return std::ranges::all_of(username, test_char);
    }
}
