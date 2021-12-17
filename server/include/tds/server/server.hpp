#pragma once

#include "tds/ip/tcp_listener.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/signal_device.hpp"
#include "tds/config/server_config.hpp"

#include <filesystem>

namespace tds::server {
    class Server {
    public:
        explicit Server(std::filesystem::path root);

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        void set_config(const config::ServerConfig& config);
        void launch();

    private:
        void stop();

        const std::filesystem::path m_root;
        config::ServerConfig m_config;

        linux::EpollDevice m_epoll_device;
        linux::SignalDevice m_signal_device;
        ip::TcpListener m_tcp_listener;
        bool m_continue;
    };
}
