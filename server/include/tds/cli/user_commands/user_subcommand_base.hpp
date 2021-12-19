#pragma once

#include <filesystem>

namespace tds::cli::user_commands {
    class UserSubcommandBase {
    public:
        const std::filesystem::path& get_user_file_path() const;
        void set_users_file_path(std::string_view users_path);

    private:
        std::filesystem::path m_users_path;
    };
}
