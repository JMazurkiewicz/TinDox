#include "tds/server/server.hpp"

#include "tds/linux/epoll_buffer.hpp"

#include <csignal>

#include <spdlog/spdlog.h>

namespace tds::server {
    Server::Server(std::filesystem::path root)
        : m_root{std::move(root)}
        , m_running{true} { }

    void Server::set_config(const config::ServerConfig& config) {
        m_config = config;
    }

    void Server::launch() {
        if(!configure()) {
            return;
        }

        for(linux::EpollBuffer buffer{4}; m_running;) {
            m_main_epoll.wait_for_events(buffer);

            for(int fd : buffer.get_available_events()) {
                if(fd == m_signal_device.get_fd()) {
                    m_signal_device.handle_last_signal();
                } else if(fd == m_tcp_listener.get_fd()) {
                    m_tcp_listener.handle_last_connection();
                } else {
                    spdlog::warn("Main epoll: unspecified file descriptor {}", fd);
                }
            }
        }
    }

    bool Server::configure() {
        try {
            configure_signals();
            configure_listener();
            configure_main_epoll();
        } catch(const std::system_error& e) {
            spdlog::error("Server error: {} (code: {})", e.what(), e.code().value());
            return false;
        }

        return true;
    }

    void Server::configure_signals() {
        const auto handler = std::bind_front(&Server::handle_stop_signal, this);
        m_signal_device.add_handler(SIGINT, handler);
        m_signal_device.add_handler(SIGTERM, handler);
        m_signal_device.apply();
    }

    void Server::configure_listener() {
        m_tcp_listener.listen(m_config.get_port());
        m_tcp_listener.set_backlog(m_config.get_backlog());
        m_tcp_listener.set_connection_handler(std::bind_front(&Server::handle_connection, this));
    }

    void Server::configure_main_epoll() {
        m_main_epoll.add_device(m_signal_device);
        m_main_epoll.add_device(m_tcp_listener);
    }

    void Server::handle_stop_signal(int code) {
        const std::string_view signal_name = [code] {
            if(code == SIGINT) {
                return "SIGINT";
            } else if(code == SIGTERM) {
                return "SIGTERM";
            } else {
                return "SIG?";
            }
        }();

        spdlog::warn("Got stop signal: {} (code {})", signal_name, code);
        stop();
    }

    void Server::handle_connection(int fd, ip::EndpointV4 client_endpoint) { }

    void Server::stop() {
        m_running = false;
    }
}
