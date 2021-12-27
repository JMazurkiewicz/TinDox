#include "tds/server/client_pool.hpp"

#include "tds/server/server_logger.hpp"

#include <mutex>
#include <stdexcept>
#include <utility>

#include <fmt/core.h>

namespace tds::server {
    void ClientPool::add_client(ip::TcpSocket socket) {
        std::lock_guard lock{m_mut};
        auto [_, inserted] = m_pool.insert({socket.get_fd(), std::make_unique<Client>(std::move(socket))});
        if(!inserted) {
            throw std::runtime_error{
                fmt::format("ClientPool failed to accept new connection (fd = {})", socket.get_fd())};
        }
    }

    std::size_t ClientPool::get_client_count() {
        std::lock_guard lock{m_mut};
        return m_pool.size();
    }

    bool ClientPool::has_client(int fd) {
        std::lock_guard lock{m_mut};
        return m_pool.find(fd) != m_pool.end();
    }

    Client& ClientPool::get_client(int fd) {
        std::lock_guard lock{m_mut};
        if(auto it = m_pool.find(fd); it != m_pool.end()) {
            return *it->second;
        } else {
            throw std::runtime_error{fmt::format("Could not find client with fd {}", fd)};
        }
    }

    void ClientPool::close_one(int fd) {
        std::lock_guard lock{m_mut};
        if(auto it = m_pool.find(fd); it != m_pool.end()) {
            m_pool.erase(it);
        } else {
            throw std::runtime_error{fmt::format("Could not close connection with client (fd = {})", fd)};
        }
    }

    void ClientPool::close_all() {
        std::lock_guard lock{m_mut};
        m_pool.clear();
    }
}
