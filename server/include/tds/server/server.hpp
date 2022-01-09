#pragma once

#include "tds/config/server_config.hpp"
#include "tds/ip/endpoint_v4.hpp"
#include "tds/ip/tcp_listener.hpp"
#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/pipe_device.hpp"
#include "tds/linux/signal_device.hpp"
#include "tds/protocol/server_context.hpp"
#include "tds/server/client_service_supervisor.hpp"

#include <filesystem>
#include <system_error>

namespace tds::server {
    class Server {
    public:
        explicit Server(std::filesystem::path root, const config::ServerConfig& config);
        ~Server();

        Server(const Server&) = delete;
        Server& operator=(const Server&) = delete;

        void launch();

    private:
        bool configure();
        void configure_signals();
        void configure_listener();
        void configure_main_epoll();

        void handle_stop_signal(int code);
        void handle_connection(ip::TcpSocket connection);

        void main_loop();
        void stop();

        const config::ServerConfig& m_config;
        protocol::ServerContext m_context;
        bool m_running;

        linux::SignalDevice m_signal_device;
        ip::TcpListener m_tcp_listener;
        linux::EpollDevice m_epoll;
        ClientServiceSupervisor m_supervisor;
    };
}
