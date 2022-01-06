#include "tds/protocol/download_token.hpp"

#include "tds/protocol/protocol_error.hpp"

namespace tds::protocol {
    DownloadToken::DownloadToken(std::filesystem::path file)
        : PathLock(std::move(file))
        , m_offset{0} {
        if(!std::filesystem::is_regular_file(file)) {
            throw ProtocolError{ProtocolCode::invalid_file_type, "DownloadToken: got invalid file path"};
        }
    }

    const std::filesystem::path& DownloadToken::get_file_path() const noexcept {
        return get_locked_path();
    }

    std::uintmax_t DownloadToken::get_file_offset() const noexcept {
        return m_offset;
    }

    void DownloadToken::set_file_offset(std::uintmax_t offset) noexcept {
        m_offset = offset;
    }

    std::shared_ptr<DownloadToken> make_download_token(std::filesystem::path file) {
        return std::make_shared<DownloadToken>(std::move(file));
    }
}
