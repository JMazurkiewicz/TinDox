#include "tds/protocol/path_lock.hpp"

#include <stdexcept>

namespace tds::protocol {
    PathLock::PathLock(std::filesystem::path path)
        : m_path{std::move(path)} {
        if(!m_path.is_absolute()) {
            throw std::runtime_error{"PathLock: path must be absolute"};
        }
    }

    bool PathLock::has_locked_path(const std::filesystem::path& path) {
        if(!path.is_absolute()) {
            throw std::runtime_error{"PathLock::has_locked_path: path must be absolute"};
        } else {
            return path.native().starts_with(m_path.native());
        }
    }

    const std::filesystem::path& PathLock::get_locked_path() const noexcept {
        return m_path;
    }

    void PathLock::set_locked_path(const std::filesystem::path& new_path) {
        m_path = std::move(new_path);
    }

    std::shared_ptr<PathLock> make_path_lock(std::filesystem::path path) {
        return std::make_shared<PathLock>(std::move(path));
    }
}
