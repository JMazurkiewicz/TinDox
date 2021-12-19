#include "tds/cli/user_commands/user_subcommand_base.hpp"

namespace tds::cli::user_commands {
    const std::filesystem::path& UserSubcommandBase::get_user_file_path() const {
        return m_users_path;
    }

    void UserSubcommandBase::set_users_file_path(std::string_view users_path) {
        m_users_path = users_path;
    }
}
