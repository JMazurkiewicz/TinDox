#pragma once

//#include "tds/server/server_context.hpp"
#include "tds/user/permissions.hpp"

#include <filesystem>

namespace tds::server {
    class ClientContext {
    public:
        ClientContext();
        ClientContext(const ClientContext&) = delete;
        ClientContext& operator=(const ClientContext&) = delete;

        bool is_alive() const noexcept;
        void kill();

        // void travel();
        // const std::filesystem::path& get_current_path();

    private:
        bool m_alive;
        // std::filesystem::path m_current_path;
    };
}
