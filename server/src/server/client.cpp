#include "tds/server/client.hpp"

#include "tds/server/server_logger.hpp"

namespace tds::server {
    Client::Client(ip::TcpSocket socket)
        : m_socket{std::move(socket)} { }

    ip::TcpSocket& Client::get_socket() noexcept {
        return m_socket;
    }

    void Client::handle_input() {
        ssize_t total_bytes = 0;
        std::array<char, 32> buffer;

        auto count = m_socket.read(buffer.data(), buffer.size());

        if(count == 0) {
            m_socket.close();
        } else {
            std::string_view str{buffer.data(), static_cast<size_t>(count)};
            server_logger->info("{}: {}", m_socket.get_endpoint(), str);
        }
    }
}
