#include "tds/server/client_pool.hpp"

#include <stdexcept>
#include <utility>

#include <fmt/core.h>

namespace tds::server {
    ClientPool::ClientPool() = default;

    void ClientPool::add_client(ip::TcpSocket socket) {
        std::lock_guard lock{m_mut};
        auto [_, inserted] = m_clients.insert(std::pair{socket.get_fd(), Client{std::move(socket)}});
        if(!inserted) {
            throw std::runtime_error{
                fmt::format("ClientPool failed to accept new connection (fd = {})", socket.get_fd())};
        }
    }

    bool ClientPool::has_client(int fd) {
        std::lock_guard lock{m_mut};
        return m_clients.find(fd) != m_clients.end();
    }

    Client& ClientPool::get_client(int fd) {
        std::lock_guard lock{m_mut};
        if(auto it = m_clients.find(fd); it != m_clients.end()) {
            return it->second;
        } else {
            throw std::runtime_error{fmt::format("No client with fd {}", fd)};
        }
    }
}
