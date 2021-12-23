#include "tds/server/server_context.hpp"

namespace tds::server {
    ServerContext::ServerContext(std::filesystem::path root)
        : m_root{std::move(root)} { }

    const std::filesystem::path& ServerContext::get_root() const noexcept {
        return m_root;
    }
}
