#include "tds/cli/user_commands/user_remove_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/linux/hash.hpp"
#include "tds/user/user_record_data.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

#include <fmt/core.h>

namespace tds::cli::user_commands {
    void UserRemoveCommand::parse_arguments(std::span<const std::string_view> args) {
        if(args.size() != 1) {
            throw InvalidCommandArgumentsError{"user remove takes username as argument", "tds user remove <username>"};
        } else {
            m_username = args[0];
        }
    }

    void UserRemoveCommand::execute() {
        std::ifstream users_file_in{get_user_file_path()};
        if(!users_file_in.is_open()) {
            throw InvalidCommandExecutionError{"program failed to open 'users' configuration file"};
        }

        bool user_removed = false;
        std::vector<user::UserRecordData> records;
        for(std::string line; std::getline(users_file_in, line);) {
            const auto record = user::make_user_record_data(line);
            if(record.get_username() != m_username) {
                records.emplace_back(std::move(record));
            } else {
                user_removed = true;
            }
        }

        if(!user_removed) {
            throw InvalidCommandExecutionError{fmt::format("no user with name '{}'", m_username)};
        }

        users_file_in.close();
        std::ofstream users_file_out{get_user_file_path()};
        if(!users_file_out.is_open()) {
            throw InvalidCommandExecutionError{"program failed to open 'users' configuration file"};
        } else {
            std::ranges::copy(records, std::ostream_iterator<user::UserRecordData>{users_file_out, "\n"});
        }
    }
}
