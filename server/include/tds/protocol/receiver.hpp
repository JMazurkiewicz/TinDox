#pragma once

#include "tds/ip/tcp_socket.hpp"

#include <span>
#include <vector>

namespace tds::protocol {
    class Receiver {
    public:
        explicit Receiver(ip::TcpSocket& socket);
        Receiver(const Receiver&) = delete;
        Receiver& operator=(const Receiver&) = delete;

        std::span<const char> receive();

    private:
        void read();
        void check_error();

        ip::TcpSocket& m_socket;
        std::vector<char> m_data;
        std::errc m_errc;
    };
}
