#pragma once

#include "tds/config/server_config.hpp"
#include "tds/ip/endpoint_v4.hpp"
#include "tds/ip/tcp_listener.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/signal_device.hpp"

#include <filesystem>
#include <system_error>

namespace tds::server {
    class Server {
    public:
        explicit Server(std::filesystem::path root);

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        void set_config(const config::ServerConfig& config);
        void launch();

    private:
        bool configure();
        void configure_signals();
        void configure_listener();
        void configure_main_epoll();

        void handle_stop_signal(int code);
        void handle_connection(int fd, ip::EndpointV4 client_endpoint);
        void stop();

        const std::filesystem::path m_root;
        bool m_running;
        config::ServerConfig m_config;

        linux::SignalDevice m_signal_device;
        ip::TcpListener m_tcp_listener;
        linux::EpollDevice m_main_epoll;
    };
}
