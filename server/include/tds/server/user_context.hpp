#pragma once

#include "tds/server/server_context.hpp"

#include <optional>
#include <string>

namespace tds::server {
    class UserContext {
    public:
        explicit UserContext(const ServerContext& server_context);

        UserContext(const UserContext&) = delete;
        UserContext& operator=(const UserContext&) = delete;

    private:
        std::string m_username;
        std::filesystem::path m_current_path;
    };
}
