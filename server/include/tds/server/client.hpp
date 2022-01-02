#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/protocol/client_context.hpp"
#include "tds/protocol/default_command_executor.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/receiver.hpp"
#include "tds/protocol/request.hpp"
#include "tds/protocol/sender.hpp"
#include "tds/protocol/server_context.hpp"

#include <queue>

namespace tds::server {
    class Client {
    public:
        explicit Client(ip::TcpSocket socket, protocol::ServerContext& server_context);
        ~Client();

        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;

        ip::TcpSocket& get_socket() noexcept;

        bool is_alive() const noexcept;
        linux::EventType get_required_events() const noexcept;
        void handle(linux::EventType events);

    private:
        void handle_input();
        void handle_commands(std::span<const char> input);
        void handle_upload(std::span<const char> input);

        void process();
        void process_commands();
        void process_download();

        void handle_output();

        ip::TcpSocket m_socket;
        protocol::ClientContext m_context;

        protocol::Receiver m_receiver;
        std::queue<protocol::Request> m_request_queue;
        protocol::ProtocolInterpreter m_interpreter;
        protocol::DefaultCommandExecutor m_command_executor;
        protocol::Sender m_sender;
    };
}
