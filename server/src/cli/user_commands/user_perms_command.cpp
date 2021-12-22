#include "tds/cli/user_commands/user_perms_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>

#include <fmt/core.h>

namespace tds::cli::user_commands {
    namespace {
        inline constexpr const char* usage = "tds user perms <username> <(+|-)(twcmdu)>...";
    }

    void UserPermsCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() < 2) {
            throw InvalidCommandArgumentsError{"too few arguments", usage};
        } else {
            m_username = args[0];
        }

        auto flags = args | std::views::drop(1) | std::views::filter(std::not_fn(std::ranges::empty));
        for(auto flag : flags) {
            auto field = [flag] {
                if(flag.front() == '-') {
                    return &UserPermsCommand::m_taken_perms;
                } else if(flag.front() == '+') {
                    return &UserPermsCommand::m_new_perms;
                } else {
                    throw InvalidCommandArgumentsError{fmt::format("invalid flag {}", flag), usage};
                }
            }();

            flag.remove_prefix(1);
            (this->*field) |= user::perms_from_string(flag);
        }
    }

    void UserPermsCommand::execute() {
        user::Permissions perms = get_user_table().get_perms_of_user(m_username);
        perms |= m_new_perms;
        perms &= ~m_taken_perms;

        get_user_table().set_perms_of_user(m_username, perms);
        std::cout << "Changed permissions for " << m_username << '\n';
    }
}
