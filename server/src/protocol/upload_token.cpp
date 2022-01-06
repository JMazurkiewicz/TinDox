#include "tds/protocol/upload_token.hpp"

namespace tds::protocol {
    UploadToken::UploadToken(const std::filesystem::path& file, std::uintmax_t file_size)
        : PathLock(file)
        , m_destination_path{file.parent_path()}
        , m_file_name{file.filename()}
        , m_size{file_size}
        , m_offset{0} { }

    const std::filesystem::path& UploadToken::get_file_path() const noexcept {
        return get_locked_path();
    }

    const std::filesystem::path& UploadToken::get_file_name() const noexcept {
        return m_file_name;
    }

    const std::filesystem::path& UploadToken::get_destination_path() const noexcept {
        return m_destination_path;
    }

    std::uintmax_t UploadToken::get_file_size() const noexcept {
        return m_size;
    }

    std::uintmax_t UploadToken::get_file_offset() const noexcept {
        return m_offset;
    }

    void UploadToken::set_file_offset(std::uintmax_t offset) noexcept {
        m_offset = offset;
    }

    std::shared_ptr<UploadToken> make_upload_token(const std::filesystem::path& file, std::uintmax_t file_size) {
        return std::make_shared<UploadToken>(file, file_size);
    }
}
