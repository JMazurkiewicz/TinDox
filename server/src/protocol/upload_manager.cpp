#include "tds/protocol/upload_manager.hpp"

#include <algorithm>

namespace tds::protocol {
    UploadManager::UploadManager(const ServerContext& server_context)
        : m_server_context{server_context} {
        m_file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    }

    UploadManager::~UploadManager() {
        if(!has_finished()) {
            create_backup();
        }
    }

    void UploadManager::start_upload(std::shared_ptr<UploadToken> token) {
        m_token = std::move(token);
        m_partial_file_name = m_server_context.get_partial_file_path(m_token->get_temporary_filename_stem());
        m_file.open(m_partial_file_name, get_openmode());

        m_backup_file_name = m_server_context.get_backup_file_path(m_token->get_temporary_filename_stem());
        if(std::filesystem::exists(m_backup_file_name)) {
            std::filesystem::remove(m_backup_file_name);
        }
    }

    std::span<const char> UploadManager::commit_bytes(std::span<const char> bytes) {
        const std::size_t byte_count = std::ranges::min(bytes.size(), get_remaining_bytes());
        m_file.write(bytes.data(), byte_count);
        m_token->add_file_offset(byte_count);

        if(byte_count == 0) {
            finish();
            return {};
        } else {
            return bytes.last(bytes.size() - byte_count);
        }
    }

    bool UploadManager::has_finished() const noexcept {
        return m_token == nullptr;
    }

    std::ios_base::openmode UploadManager::get_openmode() const noexcept {
        if(m_token->get_file_offset() != 0) {
            return std::ios_base::out | std::ios_base::app;
        } else {
            return std::ios_base::out | std::ios_base::trunc;
        }
    }

    std::size_t UploadManager::get_remaining_bytes() const noexcept {
        return m_token->get_file_size() - m_token->get_file_offset();
    }

    void UploadManager::finish() {
        m_file.close();
        std::filesystem::rename(m_partial_file_name, m_token->get_file_path());
        m_token.reset();
    }

    void UploadManager::create_backup() {
        std::ofstream backup{m_backup_file_name};
        backup << "name = " << m_token->get_file_name();
        backup << "\nsize = " << m_token->get_file_size();
        backup << "\noffset = " << m_token->get_file_offset() << '\n';
    }
}
