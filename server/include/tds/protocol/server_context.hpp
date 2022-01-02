#pragma once

#include <filesystem>

namespace tds::protocol {
    class ServerContext {
    public:
        explicit ServerContext(std::filesystem::path root);
        ServerContext(const ServerContext&) = delete;
        ServerContext& operator=(const ServerContext&) = delete;

        const std::filesystem::path& get_root_path() const noexcept;
        [[nodiscard]] std::filesystem::path get_relative_path_of(const std::filesystem::path& subpath) const;

    private:
        const std::filesystem::path m_root;
    };
}
