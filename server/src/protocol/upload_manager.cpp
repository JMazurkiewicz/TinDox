#include "tds/protocol/upload_manager.hpp"

#include "tds/protocol/server_context.hpp"

namespace tds::protocol {
    UploadManager::UploadManager(const ServerContext& server_context)
        : m_server_context{server_context} { }

    UploadManager::~UploadManager() {
        if(!has_finished()) {
            create_backup();
        }
    }

    void UploadManager::start_upload(std::shared_ptr<UploadToken> token) {
        m_token = std::move(token);
        m_file.open(m_server_context.get_partial_file_path(m_token->get_temporary_filename_stem()), get_openmode());
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

    void UploadManager::create_backup() {
        std::ofstream backup{m_server_context.get_backup_file_path(m_token->get_temporary_filename_stem())};
        backup << "destination = " << m_token->get_destination_path();
        backup << "\nsize = " << m_token->get_file_size();
        backup << "\nname = " << m_token->get_file_name();
        backup << "\noffset = " << m_token->get_file_offset() << '\n';
    }
}
