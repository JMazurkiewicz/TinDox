#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/server/client_context.hpp"

#include <vector>

namespace tds::server {
    class Client {
    public:
        explicit Client(ip::TcpSocket socket);

        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;
        Client(Client&&) = default;
        Client& operator=(Client&&) = default;

        ip::TcpSocket& get_socket() noexcept;
        bool is_alive();
        void handle();

    private:
        void read_requtests();
        void write_responses();

        ip::TcpSocket m_socket;
        bool m_alive;
        ClientContext m_context;

        std::vector<char> m_buffer;
    };
}
