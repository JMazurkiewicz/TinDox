#pragma once

#include "tds/protocol/path_lock.hpp"

namespace tds::protocol {
    class UploadToken : public PathLock {
    public:
        explicit UploadToken(const std::filesystem::path& file, std::uintmax_t file_size);

        [[nodiscard]] const std::filesystem::path& get_file_path() const noexcept;
        [[nodiscard]] const std::filesystem::path& get_file_name() const noexcept;
        [[nodiscard]] const std::filesystem::path& get_destination_path() const noexcept;
        [[nodiscard]] const std::filesystem::path& get_temporary_filename_stem() const noexcept;

        [[nodiscard]] std::uintmax_t get_file_size() const noexcept;
        [[nodiscard]] std::uintmax_t get_file_offset() const noexcept;

        void set_temporary_filename_stem(std::filesystem::path temporary_filename_stem);
        void add_file_offset(std::uintmax_t offset_change) noexcept;

    private:
        const std::filesystem::path m_destination_path;
        const std::filesystem::path m_file_name;
        std::filesystem::path m_temporary_filename_stem;
        std::uintmax_t m_size;
        std::uintmax_t m_offset;
    };

    std::shared_ptr<UploadToken> make_upload_token(const std::filesystem::path& file, std::uintmax_t file_size);
}
