#include "tds/protocol/upload_manager.hpp"

#include "tds/protocol/server_context.hpp"

namespace tds::protocol {
    UploadManager::UploadManager(const ServerContext& server_context)
        : m_temporary_path{server_context.get_config_directory_path()} { }

    UploadManager::~UploadManager() {
        create_backup();
    }

    void UploadManager::start_upload(std::shared_ptr<UploadToken> token) {
        m_token = std::move(token);
        m_file.open(get_partial_file_name(), get_openmode());
    }

    std::filesystem::path UploadManager::get_partial_file_name() const {
        return ""; // TODO USERNAME.partial
    }

    std::ios_base::openmode UploadManager::get_openmode() const noexcept {
        if(m_token->get_file_offset() != 0) {
            return std::ios_base::out | std::ios_base::app;
        } else {
            return std::ios_base::out | std::ios_base::trunc;
        }
    }

    void UploadManager::create_backup() {
        std::ofstream backup{m_temporary_path / /*USERNAME.upload*/ ""};
        backup << "destination = " << m_token->get_destination_path();
        backup << "\nsize = " << m_token->get_file_size();
        backup << "\nname = " << m_token->get_file_name();
        backup << "\noffset = " << m_token->get_file_offset() << '\n';
    }
}
