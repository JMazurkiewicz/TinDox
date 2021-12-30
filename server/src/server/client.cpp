#include "tds/server/client.hpp"

#include "tds/linux/event_type.hpp"
#include "tds/linux/linux_error.hpp"
#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/protocol_mode.hpp"
#include "tds/protocol/request.hpp"
#include "tds/protocol/response.hpp"
#include "tds/server/server_logger.hpp"

#include <exception>
#include <system_error>

namespace tds::server {
    Client::Client(ip::TcpSocket socket)
        : m_socket{std::move(socket)}
        , m_mode{protocol::ProtocolMode::command} {
        m_receiver.set_device(m_socket);
        m_sender.set_device(m_socket);
    }

    Client::~Client() {
        server_logger->warn("Closed connection with client from {}", m_socket.get_endpoint());
    }

    ip::TcpSocket& Client::get_socket() noexcept {
        return m_socket;
    }

    bool Client::is_alive() const noexcept {
        return m_context.is_alive();
    }

    linux::EventType Client::get_required_events() const noexcept {
        linux::EventType events;
        if(m_mode == protocol::ProtocolMode::command || m_mode == protocol::ProtocolMode::upload) {
            events |= linux::EventType::in;
        } else if(m_mode == protocol::ProtocolMode::download) {
            events |= linux::EventType::out;
        } else {
            __builtin_unreachable();
        }

        if(m_sender.has_responses()) {
            events |= linux::EventType::out;
        }
        return events;
    }

    void Client::handle(linux::EventType events) {
        try {
            if((events & linux::EventType::in) != linux::EventType{}) {
                handle_input();
            }

            if((events & linux::EventType::out) != linux::EventType{}) {
                handle_output();
            }
        } catch(const std::exception& e) {
            server_logger->error("Client {} caught fatal error: {}", m_socket.get_endpoint(), e.what());
            m_context.kill();
        }
    }

    void Client::handle_input() {
        const std::span<const char> input = m_receiver.receive();
        server_logger->debug("Received {} bytes from {} client", input.size(), m_socket.get_endpoint());

        switch(m_mode) {
        case protocol::ProtocolMode::command:
            handle_commands(input);
            break;

        case protocol::ProtocolMode::upload:
            // handle upload
            break;

        default:
            __builtin_unreachable(); // no input should be accepted in download mode
        }
    }

    void Client::handle_commands(std::span<const char> input) {
        try {
            do {
                input = m_interpreter.commit_bytes(input);
                if(m_interpreter.has_available_request()) {
                    const protocol::Request request = m_interpreter.get_request();
                    // TODO: push request to the queue of executor
                    m_sender.add_response(protocol::Response{std::string{request.get_name()}});
                }
            } while(!input.empty());
        } catch(const protocol::ProtocolError& e) {
            // TODO handle protocol error and send error response
        }
    }

    void Client::handle_output() {
        const std::size_t sent_byte_count = m_sender.send();
        server_logger->debug("Sent {} bytes to {} client", sent_byte_count, m_socket.get_endpoint());
    }
}
