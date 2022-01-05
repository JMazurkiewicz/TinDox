#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/file_device.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/response_builder.hpp"

#include <sys/types.h>

namespace tds::protocol {
    class DownloadManager {
    public:
        explicit DownloadManager(ip::TcpSocket& socket);
        DownloadManager(const DownloadManager&) = delete;
        DownloadManager& operator=(const DownloadManager&) = delete;

        void start_download(std::shared_ptr<DownloadToken> token);
        ssize_t send();
        bool has_finished();

    private:
        ip::TcpSocket& m_socket;
        std::shared_ptr<DownloadToken> m_token;
        linux::FileDevice m_file;

        off64_t m_offset;
        std::size_t m_file_size;
    };
}
