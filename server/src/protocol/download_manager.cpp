#include "tds/protocol/download_manager.hpp"

#include "tds/linux/linux_error.hpp"
#include "tds/linux/transfer_bytes.hpp"

#include <fcntl.h>

namespace tds::protocol {
    DownloadManager::DownloadManager(ip::TcpSocket& socket)
        : m_socket{socket} { }

    void DownloadManager::start_download(std::shared_ptr<DownloadToken> token) {
        m_token = std::move(token);
        m_file.open(m_token->get_file_path(), O_RDONLY);
        m_offset = m_token->get_file_offset();
        m_file_size = std::filesystem::file_size(m_token->get_file_path());
    }

    ssize_t DownloadManager::send() {
        const off64_t old_offset = m_offset;
        std::errc code = {};
        const int count = linux::transfer_bytes(m_file, m_socket, m_offset, m_file_size - m_offset, code);

        if(count == -1 && code != std::errc::resource_unavailable_try_again) {
            throw linux::LinuxError{static_cast<int>(code), "DownloadManager::send"};
        } else if(m_file_size - m_offset == 0) {
            m_file.close();
            m_token.reset();
        }

        return m_offset - old_offset;
    }

    bool DownloadManager::has_finished() {
        return m_token == nullptr;
    }
}
