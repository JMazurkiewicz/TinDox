#include "tds/server/client_service.hpp"

#include "tds/ip/endpoint_v4.hpp"
#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/server/server_logger.hpp"

#include <algorithm>
#include <system_error>

namespace tds::server {
    ClientService::ClientService(ClientServiceSupervisor& supervisor, const config::ServerConfig& config)
        : m_supervisor{supervisor}
        , m_epoll_buffer{std::ranges::max(static_cast<std::size_t>(config.get_max_user_count() /
                                                                   config.get_max_thread_count()),
                                          std::size_t{1})}
        , m_running{true} { }

    void ClientService::launch() {
        try {
            main_loop();
        } catch(const std::system_error& e) {
            server_logger->error("ClientService caught system error: {} ({})", e.what(), e.code());
        } catch(const std::exception& e) {
            server_logger->error("ClientService caught fatal error: {}", e.what());
        } catch(...) {
            server_logger->error("ClientService: unknown fatal error");
        }
    }

    void ClientService::main_loop() {
        while(m_running) {
            m_supervisor.wait_for_events(m_epoll_buffer);

            for(auto [fd, events] : m_epoll_buffer.get_events()) {
                if(m_supervisor.has_client(fd)) {
                    process_client_input(m_supervisor.get_client(fd), events);
                } else if(fd == m_supervisor.get_pipe_fd()) {
                    process_pipe_input();
                } else {
                    server_logger->warn("ClientService: unknown device spotted ({})", fd);
                }
            }
        }
    }

    void ClientService::process_client_input(Client& client, linux::EventType events) {
        const ip::EndpointV4 endpoint = client.get_socket().get_endpoint();

        try {
            client.handle(events);
        } catch(const std::system_error& e) {
            server_logger->error("ClientService: system error caused by client from {}: {} ({})", endpoint, e.what(),
                                 e.code());
        }

        m_supervisor.rearm_device(client.get_socket());
    }

    void ClientService::process_pipe_input() {
        server_logger->warn("ClientService: got stop signal from pipe");
        m_running = false;
    }
}
