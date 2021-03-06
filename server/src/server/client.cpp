#include "tds/server/client.hpp"

#include "tds/linux/event_type.hpp"
#include "tds/linux/linux_error.hpp"
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
        , m_sender{m_socket}
        , m_download_manager{m_socket}
        , m_upload_manager{server_context} { }

    Client::~Client() {
        server_logger->warn("Closed connection with client from {}", m_socket.get_endpoint());
    }

    ip::TcpSocket& Client::get_socket() noexcept {
        return m_socket;
    }

    bool Client::is_alive() const noexcept {
        return m_context.is_alive();
    }

    linux::EventType Client::get_required_events() const {
        const protocol::ProtocolMode mode = m_context.get_mode();
        if((!m_pending_input.empty() && mode == protocol::ProtocolMode::command) || m_sender.has_responses() ||
           mode == protocol::ProtocolMode::download) {
            return linux::EventType::out;
        } else if(mode == protocol::ProtocolMode::command || mode == protocol::ProtocolMode::upload) {
            return linux::EventType::in;
        } else {
            throw std::runtime_error{"Unexpected event required by client"};
        }
    }

    void Client::handle(linux::EventType events) {
        try {
            if(!m_pending_input.empty()) {
                handle_pending_input();
            } else if((events & linux::EventType::in) != linux::EventType{}) {
                handle_current_input();
            }

            if((events & linux::EventType::out) != linux::EventType{}) {
                handle_output();
            }
        } catch(const std::exception& e) {
            server_logger->error("Client {} caught fatal error: {}", m_socket.get_endpoint(), e.what());
            m_context.kill();
        }
    }

    void Client::handle_pending_input() {
        std::span<const char> input = m_pending_input;
        server_logger->debug("Handling {} pending bytes from {} client", input.size(), m_socket.get_endpoint());
        handle_input(input);
        m_pending_input.assign(input.begin(), input.end());
    }

    void Client::handle_current_input() {
        std::span<const char> input = m_receiver.receive();
        server_logger->debug("Received {} bytes from {} client", input.size(), m_socket.get_endpoint());
        handle_input(input);
    }

    void Client::handle_input(std::span<const char>& input) {
        do {
            switch(m_context.get_mode()) {
            case protocol::ProtocolMode::command:
                handle_request(input);
                break;

            case protocol::ProtocolMode::download:
                m_pending_input.assign(input.begin(), input.end());
                input = {};
                break;

            case protocol::ProtocolMode::upload:
                handle_upload(input);
                break;
            }
        } while(!input.empty());
    }

    void Client::handle_request(std::span<const char>& input) {
        try {
            m_interpreter.commit_bytes(input);
            if(m_interpreter.has_available_request()) {
                execute_command(m_interpreter.get_request());
            }
        } catch(const protocol::ProtocolError& e) {
            server_logger->error("Interpreter error caused by client from {}: '{}' ({})", m_socket.get_endpoint(),
                                 e.what(), static_cast<int>(e.get_code()));
            protocol::ResponseBuilder builder;
            builder.set_code(e.get_code());
            builder.set_command_name("<interpreter>");
            builder.add_line(e.what());
            m_sender.add_response(builder.get_response());
            m_interpreter.restart();
        }
    }

    void Client::handle_upload(std::span<const char>& input) {
        input = m_upload_manager.commit_bytes(input);
        if(m_upload_manager.has_finished()) {
            server_logger->info("Finished upload for {} client", m_socket.get_endpoint());
            m_context.set_mode(protocol::ProtocolMode::command);
        }
    }

    void Client::execute_command(const protocol::Request& request) {
        try {
            m_command_executor.set_command(request.get_name());
            m_command_executor.parse_fields(request.get_fields());
            server_logger->info("Executing '{}' command for {} client", request.get_name(), m_socket.get_endpoint());
            m_command_executor.execute();

            if(m_context.get_mode() == protocol::ProtocolMode::download) {
                start_download();
            } else {
                m_sender.add_response(m_command_executor.get_response());
                if(m_context.get_mode() == protocol::ProtocolMode::upload) {
                    start_upload();
                }
            }
        } catch(const protocol::ProtocolError& e) {
            server_logger->error("Execution error caused by client from {}: '{}' ({})", m_socket.get_endpoint(),
                                 e.what(), static_cast<int>(e.get_code()));
            protocol::ResponseBuilder builder;
            builder.set_code(e.get_code());
            builder.set_command_name(std::string{request.get_name()});
            builder.add_line(e.what());
            m_sender.add_response(builder.get_response());
        }
    }

    void Client::start_download() {
        auto download_token = m_context.get_download_token();
        server_logger->info("Started download of {} for {} client", download_token->get_file_path(),
                            m_socket.get_endpoint());
        m_download_manager.start_download(std::move(download_token));
    }

    void Client::start_upload() {
        auto upload_token = m_context.get_upload_token();
        server_logger->info("Started upload of {} for {} client", upload_token->get_file_path(),
                            m_socket.get_endpoint());
        m_upload_manager.start_upload(std::move(upload_token));
    }

    void Client::handle_output() {
        if(m_sender.has_responses()) {
            handle_executor_output();
        } else if(m_context.get_mode() == protocol::ProtocolMode::download) {
            handle_download_manager_output();
        }
    }

    void Client::handle_executor_output() {
        const std::size_t sent_byte_count = m_sender.send();
        server_logger->debug("Sender sent {} bytes to {} client", sent_byte_count, m_socket.get_endpoint());
    }

    void Client::handle_download_manager_output() {
        const std::size_t sent_byte_count = m_download_manager.send();
        server_logger->debug("DownloadManager sent {} bytes to {} client", sent_byte_count, m_socket.get_endpoint());
        if(m_download_manager.has_finished()) {
            server_logger->info("Finished download for {} client", m_socket.get_endpoint());
            m_context.set_mode(protocol::ProtocolMode::command);
        }
    }
}
