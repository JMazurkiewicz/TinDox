#include "tds/protocol/download_manager.hpp"

#include "tds/linux/linux_error.hpp"

#include <stdexcept>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace tds::protocol {
    DownloadManager::DownloadManager(ip::TcpSocket& socket)
        : m_socket{socket}
        , m_file{nullptr} { }

    DownloadManager::~DownloadManager() {
        if(m_file != nullptr) {
            std::fclose(m_file);
        }
    }

    void DownloadManager::start_download(std::shared_ptr<DownloadToken> token) {
        m_token = std::move(token);
        open_file();
        init_state();
    }

    ssize_t DownloadManager::send() {
        const off64_t old_offset = m_offset;
        const int result = sendfile64(m_socket.get_fd(), fileno(m_file), &m_offset, m_file_size - m_offset);

        if(result == -1 && errno != EAGAIN) {
            throw linux::LinuxError{"sendfile64(2)"};
        } else if(m_file_size - m_offset == 0) {
            close_file();
            m_token.reset();
        }

        return m_offset - old_offset;
    }

    bool DownloadManager::has_finished() {
        return m_token == nullptr;
    }

    void DownloadManager::open_file() {
        const fs::path& path = m_token->get_file_path();
        m_file = std::fopen(path.c_str(), "rb");
        if(m_file == nullptr) {
            throw std::runtime_error{fmt::format("DownloadManager: failed to open '{}' file", path)};
        }
    }

    void DownloadManager::init_state() {
        m_offset = m_token->get_file_offset();
        m_file_size = fs::file_size(m_token->get_file_path());
    }

    void DownloadManager::close_file() {
        if(m_file != nullptr) {
            std::fclose(m_file);
            m_file = nullptr;
        }
    }
}
