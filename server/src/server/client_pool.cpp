#include "tds/server/client_pool.hpp"

#include <fmt/core.h>

namespace tds::server {
    ClientPool::ClientPool(protocol::ServerContext& server_context)
        : m_server_context{server_context} { }

    void ClientPool::spawn_client(ip::TcpSocket socket) {
        std::scoped_lock lock{m_mut};
        const int socket_fd = socket.get_fd();
        auto [_, inserted] = m_pool.insert({socket_fd, std::make_unique<Client>(std::move(socket), m_server_context)});
        if(!inserted) {
            throw std::runtime_error{
                fmt::format("ClientPool: failed to accept new connection (fd = {})", socket.get_fd())};
        }
    }

    std::size_t ClientPool::get_client_count() const {
        std::shared_lock lock{m_mut};
        return m_pool.size();
    }

    bool ClientPool::has_client(int fd) const {
        std::shared_lock lock{m_mut};
        return m_pool.find(fd) != m_pool.end();
    }

    Client& ClientPool::get_client(int fd) const {
        std::shared_lock lock{m_mut};
        if(auto it = m_pool.find(fd); it != m_pool.end()) {
            return *it->second;
        } else {
            throw std::runtime_error{fmt::format("ClientPool: could not find client with fd {}", fd)};
        }
    }

    void ClientPool::close_one(int fd) {
        std::scoped_lock lock{m_mut};
        if(auto it = m_pool.find(fd); it != m_pool.end()) {
            m_pool.erase(it);
        } else {
            throw std::runtime_error{fmt::format("ClientPool: could not close connection with client (fd = {})", fd)};
        }
    }

    void ClientPool::close_all() {
        std::scoped_lock lock{m_mut};
        m_pool.clear();
    }
}
