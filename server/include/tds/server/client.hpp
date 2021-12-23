#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/server/client_context.hpp"

namespace tds::server {
    class Client {
    public:
        explicit Client(ip::TcpSocket socket);

        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;
        Client(Client&&) = default;
        Client& operator=(Client&&) = default;

        ip::TcpSocket& get_socket() noexcept;
        void handle_input();

    private:
        ip::TcpSocket m_socket;
        ClientContext m_context;
    };
}
