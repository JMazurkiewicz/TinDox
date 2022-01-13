#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/protocol/server_context.hpp"
#include "tds/server/client.hpp"

#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace tds::server {
    class ClientPool {
    public:
        explicit ClientPool(protocol::ServerContext& server_context);
        ClientPool(const ClientPool&) = delete;
        ClientPool& operator=(const ClientPool&) = delete;

        void spawn_client(ip::TcpSocket socket);

        std::size_t get_client_count() const;
        bool has_client(int fd) const;
        Client& get_client(int fd) const;

        void close_one(int fd);
        void close_all();

    private:
        protocol::ServerContext& m_server_context;
        mutable std::shared_mutex m_mut;
        std::unordered_map<int, std::unique_ptr<Client>> m_pool;
    };
}
