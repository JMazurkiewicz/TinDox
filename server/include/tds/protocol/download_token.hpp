#pragma once

#include "tds/user/permissions.hpp"

#include <filesystem>
#include <string>

namespace tds::protocol {
    class DownloadToken {
    public:
        explicit DownloadToken(std::filesystem::path file_path);
        DownloadToken(const DownloadToken&) = delete;
        DownloadToken& operator=(const DownloadToken&) = delete;

        const std::filesystem::path& get_file_path() const noexcept;
        std::uintmax_t get_file_offset() const noexcept;

        void set_file_offset(std::uintmax_t offset) noexcept;

    private:
        std::filesystem::path m_path;
        std::uintmax_t m_offset;
    };
}
