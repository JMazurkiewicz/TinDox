#include "tds/protocol/download_token.hpp"

namespace tds::protocol {
    DownloadToken::DownloadToken(std::filesystem::path file_path)
        : m_path{std::move(file_path)}
        , m_offset{0} { }

    const std::filesystem::path& DownloadToken::get_file_path() const noexcept {
        return m_path;
    }

    std::uintmax_t DownloadToken::get_file_offset() const noexcept {
        return m_offset;
    }

    void DownloadToken::set_file_offset(std::uintmax_t offset) noexcept {
        m_offset = offset;
    }
}
