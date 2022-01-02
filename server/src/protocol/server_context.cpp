#include "tds/protocol/server_context.hpp"

namespace fs = std::filesystem;

namespace tds::protocol {
    ServerContext::ServerContext(fs::path root)
        : m_root{std::move(root)} { }

    const fs::path& ServerContext::get_root_path() const noexcept {
        return m_root;
    }

    fs::path ServerContext::get_relative_path_of(const fs::path& subpath) const {
        fs::path result = fs::relative(subpath, get_root_path());
        if(result == ".") {
            result.clear();
        }
        return fs::path{"/"} / std::move(result);
    }
}
