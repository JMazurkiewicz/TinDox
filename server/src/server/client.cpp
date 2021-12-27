#include "tds/server/client.hpp"

#include "tds/linux/event_type.hpp"
#include "tds/linux/linux_error.hpp"
#include "tds/protocol/response.hpp"
#include "tds/server/server_logger.hpp"

#include <exception>

namespace tds::server {
    Client::Client(ip::TcpSocket socket)
        : m_socket{std::move(socket)}
        , m_alive{true} {
        m_receiver.set_device(m_socket);
        m_sender.set_device(m_socket);
    }

    ip::TcpSocket& Client::get_socket() noexcept {
        return m_socket;
    }

    int Client::get_fd() const noexcept {
        return m_socket.get_fd();
    }

    bool Client::is_alive() const noexcept {
        return m_alive;
    }

    linux::EventType Client::get_required_events() const noexcept {
        auto events = linux::EventType::in;
        if(m_sender.has_responses()) {
            events |= linux::EventType::out;
        }
        return events;
    }

    void Client::handle() {
        try {
            const auto data = m_receiver.receive();
            server_logger->info("Received {} bytes from {} client", data.size(), m_socket.get_endpoint());

            protocol::Response response{
                std::string{data.data(), data.size()}
            };
            m_sender.add_response(std::move(response));
            const auto count = m_sender.send();
            server_logger->info("Sent {} bytes to {} client", count, m_socket.get_endpoint());
        } catch(const std::exception&) {
            server_logger->error("Client {} caught fatal error", m_socket.get_endpoint());
            m_alive = false;
        }
    }
}
