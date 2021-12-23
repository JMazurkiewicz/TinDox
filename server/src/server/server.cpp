#include "tds/server/server.hpp"

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/terminal.hpp"
#include "tds/server/server_logger.hpp"

#include <csignal>
#include <cstring>
#include <stdexcept>

namespace tds::server {
    Server::Server(std::filesystem::path root)
        : m_root{std::move(root)}
        , m_running{true} { }

    Server::~Server() {
        stop();
    }

    void Server::set_config(const config::ServerConfig& config) {
        m_config = config;
    }

    void Server::launch() {
        if(configure()) {
            server_logger->info("Server: starting");
            main_loop();
        }
    }

    bool Server::configure() {
        try {
            server_logger->info("Server: basic configuration");
            configure_signals();
            configure_listener();
            configure_main_epoll();
            configure_client_service();
            linux::Terminal::set_stdin_echo(false);
        } catch(const std::system_error& e) {
            server_logger->error("Configuration failed: {} ({})", e.what(), e.code());
            return false;
        }

        return true;
    }

    void Server::configure_signals() {
        const auto handler = std::bind_front(&Server::handle_stop_signal, this);
        m_signal_device.add_handler(SIGINT, handler);
        m_signal_device.add_handler(SIGQUIT, handler);
        m_signal_device.add_handler(SIGTERM, handler);
        m_signal_device.apply();
    }

    void Server::configure_listener() {
        m_tcp_listener.listen(m_config.get_port());
        m_tcp_listener.set_backlog(m_config.get_backlog());
        m_tcp_listener.set_connection_type(ip::SocketType::nonblocking);
        m_tcp_listener.set_connection_handler(std::bind_front(&Server::handle_connection, this));
    }

    void Server::configure_main_epoll() {
        m_main_epoll.add_device(m_signal_device);
        m_main_epoll.add_device(m_tcp_listener);
    }

    void Server::configure_client_service() {
        m_supervisor.set_config(m_config);
        m_supervisor.create_services();
    }

    void Server::handle_stop_signal(int code) {
        server_logger->warn("Got stop signal: {} (code {})", strsignal(code), code);
        stop();
    }

    void Server::handle_connection(ip::TcpSocket connection) {
        const ip::EndpointV4 endpoint = connection.get_endpoint();

        try {
            server_logger->info("New connection from {} (fd = {})", endpoint, connection.get_fd());
            m_supervisor.add_connection(std::move(connection));
        } catch(const std::system_error& e) {
            server_logger->error("Failed to add connection from {}: {} ({})", endpoint, e.what(), e.code());
        } catch(const std::runtime_error& e) {
            server_logger->error("Failed to add connection from {}: {}", endpoint, e.what());
        }
    }

    void Server::main_loop() {
        for(linux::EpollBuffer buffer{4}; m_running;) {
            m_main_epoll.wait_for_events(buffer);

            for(auto [fd, events] : buffer.get_events()) {
                if(fd == m_signal_device.get_fd()) {
                    m_signal_device.handle_signal();
                } else if(fd == m_tcp_listener.get_fd()) {
                    m_tcp_listener.handle_connection();
                } else {
                    server_logger->warn("Main epoll: unknown file descriptor {}", fd);
                }
            }
        }
    }

    void Server::stop() {
        if(m_running) {
            server_logger->warn("Server: stop requested");
            m_running = false;
            m_supervisor.stop();
        }
    }
}
