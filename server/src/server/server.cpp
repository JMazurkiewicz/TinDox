#include "tds/server/server.hpp"

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/echoless_terminal_guard.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/server/server_logger.hpp"

#include <csignal>
#include <cstring>
#include <stdexcept>

namespace tds::server {
    Server::Server(std::filesystem::path root, const config::ServerConfig& config)
        : m_config{config}
        , m_context{std::move(root), m_config}
        , m_running{true}
        , m_supervisor{m_context} { }

    Server::~Server() {
        stop();
    }

    void Server::launch() {
        if(configure()) {
            server_logger->info("Server: starting");
            linux::EcholessTerminalGuard terminal_guard;
            main_loop();
        }
    }

    bool Server::configure() {
        try {
            configure_signals();
            configure_listener();
            configure_main_epoll();
            m_supervisor.create_services(m_config);
        } catch(const std::system_error& e) {
            server_logger->error("Configuration failed: {} ({})", e.what(), e.code());
            return false;
        }

        return true;
    }

    void Server::configure_signals() {
        server_logger->info("Server: creating signal handlers");
        const auto handler = std::bind_front(&Server::handle_stop_signal, this);
        m_signal_device.add_handler(SIGINT, handler);
        m_signal_device.add_handler(SIGQUIT, handler);
        m_signal_device.add_handler(SIGTERM, handler);
        m_signal_device.apply();
    }

    void Server::configure_listener() {
        const ip::Port port = m_config.get_port();
        const int backlog = m_config.get_backlog();

        server_logger->info("Server: creating TCP listener (port = {}, backlog = {})", port, backlog);
        m_tcp_listener.listen(port);
        m_tcp_listener.set_backlog(backlog);
        m_tcp_listener.set_connection_type(ip::SocketType::nonblocking);
        m_tcp_listener.set_connection_handler(std::bind_front(&Server::handle_connection, this));
    }

    void Server::configure_main_epoll() {
        server_logger->info("Server: configuring main epoll device");
        m_epoll.add_device(m_signal_device);
        m_epoll.add_device(m_tcp_listener);
    }

    void Server::handle_stop_signal(int code) {
        server_logger->warn("Got stop signal: {} (code {})", strsignal(code), code);
        stop();
    }

    void Server::handle_connection(ip::TcpSocket connection) {
        const ip::EndpointV4 endpoint = connection.get_endpoint();

        try {
            server_logger->info("New connection from {}", endpoint, connection.get_fd());

            if(m_supervisor.get_client_count() < m_config.get_max_user_count()) {
                m_supervisor.accept_connection(std::move(connection));
            } else {
                server_logger->error("Server: too many clients, cannot accept {}", endpoint);
            }

        } catch(const std::system_error& e) {
            server_logger->error("Failed to add connection from {}: {} ({})", endpoint, e.what(), e.code());
        } catch(const std::runtime_error& e) {
            server_logger->error("Failed to add connection from {}: {}", endpoint, e.what());
        }
    }

    void Server::main_loop() {
        for(linux::EpollBuffer buffer{16}; m_running;) {
            m_epoll.wait_for_events(buffer);

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
