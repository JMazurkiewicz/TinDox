#pragma once

#include "tds/server/server_context.hpp"

namespace tds::server {
    class UserContext {
    public:
        explicit UserContext(const ServerContext& server_context);

        UserContext(const UserContext&) = delete;
        UserContext& operator=(const UserContext&) = delete;

    private:
        std::filesystem::path m_current_path;
    };
}
