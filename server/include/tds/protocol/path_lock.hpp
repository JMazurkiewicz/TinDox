#pragma once

#include <filesystem>
#include <memory>

namespace tds::protocol {
    class PathLock {
    public:
        explicit PathLock(std::filesystem::path path);
        PathLock(const PathLock&) = delete;
        PathLock& operator=(const PathLock&) = delete;

        bool has_locked_path(const std::filesystem::path& path);
        const std::filesystem::path& get_locked_path() const noexcept;

    private:
        std::filesystem::path m_path;
    };

    std::shared_ptr<PathLock> make_path_lock(std::filesystem::path path);
}
