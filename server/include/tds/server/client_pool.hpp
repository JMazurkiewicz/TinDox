#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/server/client.hpp"

#include <mutex>
#include <unordered_map>

namespace tds::server {
    class ClientPool {
    public:
        ClientPool();
        ClientPool(const ClientPool&) = delete;
        ClientPool& operator=(const ClientPool&) = delete;

        void add_client(ip::TcpSocket socket);

        std::size_t get_client_count();
        bool has_client(int fd);
        Client& get_client(int fd);

        void close_one(int fd);
        void close_all();

    private:
        std::mutex m_mut;
        std::unordered_map<int, Client> m_pool;
    };
}
