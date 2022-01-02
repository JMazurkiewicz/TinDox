#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/protocol/server_context.hpp"
#include "tds/server/client.hpp"

#include <memory>
#include <mutex>
#include <unordered_map>

namespace tds::server {
    class ClientPool {
    public:
        explicit ClientPool(const protocol::ServerContext& server_context);
        ClientPool(const ClientPool&) = delete;
        ClientPool& operator=(const ClientPool&) = delete;

        void add_client(ip::TcpSocket socket);

        std::size_t get_client_count();
        bool has_client(int fd);
        Client& get_client(int fd);

        void close_one(int fd);
        void close_all();

    private:
        const protocol::ServerContext& m_server_context;
        std::mutex m_mut;
        std::unordered_map<int, std::unique_ptr<Client>> m_pool;
    };
}
