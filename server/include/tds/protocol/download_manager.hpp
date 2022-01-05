#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/protocol/download_token.hpp"
#include "tds/protocol/response_builder.hpp"

#include <cstddef>
#include <cstdio>

#include <sys/sendfile.h>

namespace tds::protocol {
    class DownloadManager {
    public:
        explicit DownloadManager(ip::TcpSocket& socket);
        DownloadManager(const DownloadManager&) = delete;
        DownloadManager& operator=(const DownloadManager&) = delete;
        ~DownloadManager();

        void start_download(std::shared_ptr<DownloadToken> token);
        ssize_t send();
        bool has_finished();

    private:
        void open_file();
        void init_state();
        void close_file();

        ip::TcpSocket& m_socket;
        std::shared_ptr<DownloadToken> m_token;
        std::FILE* m_file;

        off64_t m_offset;
        std::size_t m_file_size;
    };
}
