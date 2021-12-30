#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/protocol_mode.hpp"
#include "tds/protocol/receiver.hpp"
#include "tds/protocol/request.hpp"
#include "tds/protocol/sender.hpp"
#include "tds/server/client_context.hpp"

namespace tds::server {
    class Client {
    public:
        explicit Client(ip::TcpSocket socket);
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

        ip::TcpSocket m_socket;
        ClientContext m_context;
        linux::EventType m_required_events;

        protocol::ProtocolMode m_mode;
        protocol::ProtocolInterpreter m_interpreter;

        protocol::Receiver m_receiver;
        // std::queue<protocol::Request> m_requests; TODO request queue will be in protocol::Executor!!!

        protocol::Sender m_sender;
    };
}
