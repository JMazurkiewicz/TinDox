#pragma once

#include <filesystem>

namespace tds::server {
    class ServerContext {
    public:
        explicit ServerContext(std::filesystem::path root);
        ServerContext(const ServerContext&) = delete;
        ServerContext& operator=(const ServerContext&) = delete;

        const std::filesystem::path& get_root() const noexcept;

    private:
        const std::filesystem::path m_root;
    };
}
