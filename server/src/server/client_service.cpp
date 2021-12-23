#include "tds/server/client_service.hpp"

#include "tds/server/server_logger.hpp"

namespace tds::server {
    ClientService::ClientService(ClientServiceSupervisor& supervisor)
        : m_supervisor{&supervisor}
        , m_epoll_buffer{32}
        , m_running{true} { }

    void ClientService::launch() {
        while(m_running) {
            process_events();
        }
    }
    void ClientService::process_events() {
        m_supervisor->wait_for_events(m_epoll_buffer);

        for(auto [fd, events] : m_epoll_buffer.get_events()) {
            if(m_supervisor->has_client(fd)) {
                process_client_input(m_supervisor->get_client(fd));
            } else if(fd == m_supervisor->get_pipe_fd()) {
                process_pipe_input();
            } else {
                server_logger->warn("Unknown device spotted ({})", fd);
            }
        }
    }

    void ClientService::process_client_input(Client& client) {
        client.handle_input();
        m_supervisor->rearm_device(client.get_socket());
    }

    void ClientService::process_pipe_input() {
        server_logger->warn("Got stop signal from pipe ({})", m_supervisor->get_pipe_fd());
        m_running = false;
    }
}
