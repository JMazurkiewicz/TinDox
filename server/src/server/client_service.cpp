#include "tds/server/client_service.hpp"

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/server/server_logger.hpp"

#include <system_error>

namespace tds::server {
    ClientService::ClientService(ClientServiceSupervisor& supervisor)
        : m_supervisor{&supervisor}
        , m_running{true} { }

    void ClientService::launch() {
        for(linux::EpollBuffer buffer{32}; m_running;) {
            m_supervisor->wait_for_events(buffer);

            for(auto [fd, events] : buffer.get_events()) {
                if(m_supervisor->has_client(fd)) {
                    process_client_input(m_supervisor->get_client(fd));
                } else if(fd == m_supervisor->get_pipe_fd()) {
                    process_pipe_input();
                } else {
                    server_logger->warn("ClientService: unknown device spotted ({})", fd);
                }
            }
        }
    }

    void ClientService::process_client_input(Client& client) {
        ip::TcpSocket& socket = client.get_socket();

        try {
            client.handle();
        } catch(const std::system_error& e) {
            server_logger->error("ClientService: error caused by client from {} (fd = {}): {} ({})",
                                 socket.get_endpoint(), socket.get_fd(), e.what(), e.code());
        } catch(const std::exception& e) {
            server_logger->error("ClientService: error caused by client from {} (fd = {}): {}", socket.get_endpoint(),
                                 socket.get_fd(), e.what());
        } catch(...) {
            server_logger->error("ClientService: unknown fatal error caused by client from {} (fd = {})",
                                 socket.get_endpoint(), client.get_socket().get_fd());
        }

        m_supervisor->rearm_device(client.get_socket());
    }

    void ClientService::process_pipe_input() {
        server_logger->warn("ClientService: got stop signal from pipe ({})", m_supervisor->get_pipe_fd());
        m_running = false;
    }
}
