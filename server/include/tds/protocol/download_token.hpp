#pragma once

#include "tds/protocol/path_lock.hpp"
#include "tds/user/permissions.hpp"

#include <filesystem>
#include <string>

namespace tds::protocol {
    class DownloadToken : public PathLock {
    public:
        DownloadToken(std::filesystem::path file);

        const std::filesystem::path& get_file_path() const noexcept;
        std::uintmax_t get_file_offset() const noexcept;

        void set_file_offset(std::uintmax_t offset) noexcept;

    private:
        std::uintmax_t m_offset;
    };

    std::shared_ptr<DownloadToken> make_download_token(std::filesystem::path file_to_download);
}
