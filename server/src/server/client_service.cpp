#include "tds/server/client_service.hpp"

namespace tds::server {
    ClientService::ClientService(ClientServiceSupervisor& supervisor)
        : m_supervisor{&supervisor}
        , m_epoll_buffer{32}
        , m_running{true} { }

    void ClientService::operator()() {
        while(m_running) {
            wait_for_events();
            process_events();
        }
    }

    void ClientService::wait_for_events() { }

    void ClientService::process_events() {
        for(auto [fd, events] : m_epoll_buffer.get_events()) {
            if(m_supervisor->has_client(fd)) {
                m_supervisor->get_client(fd).handle();
            } else if(fd == m_supervisor->get_pipe_fd()) {
                process_pipe_request();
            } else {
                // whaaat
            }
        }
    }

    void ClientService::process_pipe_request() {
        m_running = false;
    }
}
