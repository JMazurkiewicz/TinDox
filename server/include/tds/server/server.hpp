#pragma once

#include "tds/ip/tcp_listener.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/signal_device.hpp"

namespace tds::server {
    class Server {
    public:
        explicit Server();

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        void launch();

    private:
        void stop();

        linux::EpollDevice m_epoll_device;
        linux::SignalDevice m_signal_device;
        ip::TcpListener m_tcp_listener;
        bool m_continue;
    };
}
