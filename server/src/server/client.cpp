#include "tds/server/client.hpp"

#include "tds/linux/event_type.hpp"
#include "tds/linux/linux_error.hpp"
#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/protocol_mode.hpp"
#include "tds/protocol/request.hpp"
#include "tds/protocol/response.hpp"
#include "tds/protocol/response_builder.hpp"
#include "tds/server/server_logger.hpp"

namespace tds::server {
    Client::Client(ip::TcpSocket socket, protocol::ServerContext& server_context)
        : m_socket{std::move(socket)}
        , m_receiver{m_socket}
        , m_command_executor{server_context, m_context}
        , m_sender{m_socket} { }

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
        linux::EventType events = {};
        protocol::ProtocolMode mode = m_context.get_mode();
        if(mode == protocol::ProtocolMode::command || mode == protocol::ProtocolMode::upload) {
            events |= linux::EventType::in;
        } else if(mode == protocol::ProtocolMode::download) {
            events |= linux::EventType::out;
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
            process();
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

        switch(m_context.get_mode()) {
        case protocol::ProtocolMode::command:
            handle_commands(input);
            break;

        case protocol::ProtocolMode::upload:
            handle_upload(input);
            break;

        default:
            __builtin_unreachable(); // no input should be accepted in download mode
        }
    }

    void Client::handle_commands(std::span<const char> input) {
        do {
            try {
                input = m_interpreter.commit_bytes(input);
                if(m_interpreter.has_available_request()) {
                    m_request_queue.push(m_interpreter.get_request());
                }
            } catch(const protocol::ProtocolError& e) {
                protocol::ResponseBuilder builder;
                builder.set_code(e.get_code());
                builder.set_command_name("<interpreter>");
                builder.add_line(e.what());
                m_sender.add_response(builder.get_response());
                m_interpreter.restart();
            }

            // TODO very special case -- break if upload start/resume happened
        } while(!input.empty());

        if(!input.empty()) {
            handle_upload(input);
        }
    }

    void Client::handle_upload(std::span<const char> input) {
        // TODO upload file with UploadManager!
    }

    void Client::process() {
        switch(m_context.get_mode()) {
        case protocol::ProtocolMode::command:
            process_commands();
            break;

        case protocol::ProtocolMode::download:
            process_download();
            break;

        case protocol::ProtocolMode::upload:
            // TODO process upload??
            break;
        }
    }

    void Client::process_commands() {
        while(m_context.get_mode() == protocol::ProtocolMode::command && !m_request_queue.empty()) {
            const protocol::Request& request = m_request_queue.front();

            try {
                m_command_executor.set_command(request.get_name());
                m_command_executor.parse_fields(request.get_fields());
                server_logger->debug("Executing '{}' command for {} client", request.get_name(),
                                     m_socket.get_endpoint());
                m_command_executor.execute();
                m_sender.add_response(m_command_executor.get_response());
            } catch(const protocol::ProtocolError& e) {
                server_logger->debug("Execution error caused by client from {}: '{}' ({})", m_socket.get_endpoint(),
                                     e.what(), static_cast<int>(e.get_code()));
                protocol::ResponseBuilder builder;
                builder.set_code(e.get_code());
                builder.set_command_name(std::string{request.get_name()});
                builder.add_line(e.what());
                m_sender.add_response(builder.get_response());
            }

            m_request_queue.pop();
        }
    }

    void Client::process_download() {
        // TODO handle download with download manager
    }

    void Client::handle_output() {
        const std::size_t sent_byte_count = m_sender.send();
        server_logger->debug("Sent {} bytes to {} client", sent_byte_count, m_socket.get_endpoint());
    }
}
