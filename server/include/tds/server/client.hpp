#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/protocol/interpreter_mode.hpp"
#include "tds/protocol/receiver.hpp"
#include "tds/protocol/sender.hpp"
#include "tds/server/client_context.hpp"

namespace tds::server {
    class Client {
    public:
        explicit Client(ip::TcpSocket socket);

        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;

        ip::TcpSocket& get_socket() noexcept;
        int get_fd() const noexcept;

        bool is_alive() const noexcept;
        linux::EventType get_required_events() const noexcept;
        void handle();

    private:
        ip::TcpSocket m_socket;
        ClientContext m_context;

        bool m_alive;
        linux::EventType m_required_events;

        protocol::Receiver m_receiver;
        protocol::Sender m_sender;
    };
}
