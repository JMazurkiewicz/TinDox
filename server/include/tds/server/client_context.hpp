#pragma once

//#include "tds/server/server_context.hpp"
#include "tds/user/permissions.hpp"

#include <filesystem>

namespace tds::server {
    class ClientContext {
    public:
        ClientContext() = default;
        ClientContext(const ClientContext&) = delete;
        ClientContext& operator=(const ClientContext&) = delete;
        ClientContext(ClientContext&&) = default;
        ClientContext& operator=(ClientContext&&) = default;

        // void travel();
        // const std::filesystem::path& get_current_path();

    private:
        // std::filesystem::path m_current_path;
    };
}
