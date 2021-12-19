#include "tds/cli/user_commands/user_passwd_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/config/limits.hpp"
#include "tds/linux/hash.hpp"
#include "tds/linux/terminal.hpp"
#include "tds/user/permissions.hpp"
#include "tds/user/user_record_data.hpp"

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
                fmt::format("too many attempts (max is {})", config::limits::max_try_count)};
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
        find_old_password();
        read_old_password();
        read_new_password();
        read_new_password_repeated();
        write_new_credentials();
    }

    void UserPasswdCommand::find_old_password() {
        std::ifstream users_file{get_user_file_path()};
        if(!users_file.is_open()) {
            throw InvalidCommandExecutionError{"program failed to open 'users' configuration file"};
        }

        for(std::string line; std::getline(users_file, line);) {
            const auto record = user::make_user_record_data(line);
            if(record.get_username() == m_username) {
                m_old_password_hash = record.get_password_hash();
                return;
            }
        }

        throw InvalidCommandExecutionError{fmt::format("user with name '{}' not found", m_username)};
    }

    void UserPasswdCommand::read_old_password() const {
        linux::Terminal::set_stdin_echo(false);
        std::string old_password;
        for(int i = 0; i < config::limits::max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter old password: ";
            } else {
                std::cout << "Wrong password, try again: ";
            }

            std::getline(std::cin, old_password);
            std::cout << '\n';
            if(linux::hash(old_password) == m_old_password_hash) {
                linux::Terminal::set_stdin_echo(true);
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserPasswdCommand::read_new_password() {
        linux::Terminal::set_stdin_echo(false);
        for(int i = 0; i < config::limits::max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Enter new password: ";
            } else {
                std::cout << "New password is too long, try something shorter: ";
            }

            std::getline(std::cin, m_new_password);
            std::cout << '\n';
            if(m_new_password.size() <= config::limits::max_password_length) {
                linux::Terminal::set_stdin_echo(true);
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserPasswdCommand::read_new_password_repeated() const {
        linux::Terminal::set_stdin_echo(false);
        std::string repeated_password;
        for(int i = 0; i < config::limits::max_try_count; ++i) {
            if(i == 0) {
                std::cout << "Repeat new password: ";
            } else {
                std::cout << "Passwords don't match. Try again: ";
            }

            std::getline(std::cin, repeated_password);
            std::cout << '\n';
            if(repeated_password == m_new_password) {
                linux::Terminal::set_stdin_echo(false);
                return;
            }
        }

        throw_too_many_attempts_error();
    }

    void UserPasswdCommand::write_new_credentials() const {
        std::ifstream users_file_in{get_user_file_path()};
        if(!users_file_in.is_open()) {
            throw InvalidCommandExecutionError{"program failed to open 'users' configuration file"};
        }

        std::vector<user::UserRecordData> records;
        user::Permissions perms;

        for(std::string line; std::getline(users_file_in, line);) {
            const auto record = user::make_user_record_data(line);
            if(record.get_username() == m_username) {
                perms = record.get_permissions();
            } else {
                records.emplace_back(std::move(record));
            }
        }

        users_file_in.close();
        std::ofstream users_file_out{get_user_file_path()};
        if(!users_file_out.is_open()) {
            throw InvalidCommandExecutionError{"program failed to open 'users' configuration file"};
        }

        std::ranges::copy(records, std::ostream_iterator<user::UserRecordData>{users_file_out, "\n"});
        users_file_out << m_username << ':' << linux::hash(m_new_password) << ':' << perms << '\n';
    }

}
