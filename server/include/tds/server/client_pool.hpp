#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/server/client.hpp"

#include <unordered_map>

namespace tds::server {
    class ClientPool {
    public:
        ClientPool();
        ClientPool(const ClientPool&) = delete;
        ClientPool& operator=(const ClientPool&) = delete;

        void add_client(ip::TcpSocket socket);

        bool has_client(int fd) const;
        Client& get_client(int fd);

    private:
        std::unordered_map<int, Client> m_clients;
    };
}
