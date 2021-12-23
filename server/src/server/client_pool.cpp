#include "tds/server/client_pool.hpp"

#include <stdexcept>
#include <utility>

#include <fmt/core.h>

namespace tds::server {
    ClientPool::ClientPool() = default;

    void ClientPool::add_client(ip::TcpSocket socket) {
        std::lock_guard lock{m_mut};
        auto [_, inserted] = m_pool.insert(std::pair{socket.get_fd(), Client{std::move(socket)}});
        if(!inserted) {
            throw std::runtime_error{
                fmt::format("ClientPool failed to accept new connection (fd = {})", socket.get_fd())};
        }
    }

    bool ClientPool::has_client(int fd) {
        std::lock_guard lock{m_mut};
        return m_pool.find(fd) != m_pool.end();
    }

    Client& ClientPool::get_client(int fd) {
        std::lock_guard lock{m_mut};
        if(auto it = m_pool.find(fd); it != m_pool.end()) {
            return it->second;
        } else {
            throw std::runtime_error{fmt::format("No client with fd {}", fd)};
        }
    }

    void ClientPool::close_one(int fd) {
        std::lock_guard lock{m_mut};
        if(m_pool.erase(fd) == 0) {
            throw std::runtime_error{fmt::format("ClientPool failed to remove client (fd = {})", fd)};
        }
    }

    void ClientPool::close_all() {
        std::lock_guard lock{m_mut};
        m_pool.clear();
    }
}
