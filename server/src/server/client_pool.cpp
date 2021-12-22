#include "tds/server/client_pool.hpp"

namespace tds::server {
    ClientPool::ClientPool() = default;

    void ClientPool::add_client(ip::TcpSocket socket) { }

    bool ClientPool::has_client(int fd) const {
        return false;
    }

    Client& ClientPool::get_client(int fd) {
        // !!!
    }
}
