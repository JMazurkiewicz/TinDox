#include "tds/server/client.hpp"

namespace tds::server {
    Client::Client(ip::TcpSocket socket)
        : m_socket{std::move(socket)} { }

    int Client::get_fd() const noexcept {
        return m_socket.get_fd();
    }

    void Client::handle() { }
}
