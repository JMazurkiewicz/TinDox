#include "tds/server/connection_service.hpp"

#include <spdlog/spdlog.h>

namespace tds::server {
    ConnectionService::ConnectionService(ConnectionServiceSupervisor& supervisor)
        : m_supervisor{supervisor}
        , m_epoll_buffer{32}
        , m_running{true} { }

    void ConnectionService::operator()() {
        while(m_running) {
            wait_for_events();
            process_events();
        }
    }

    void ConnectionService::wait_for_events() { }

    void ConnectionService::process_events() {
        for(int fd : m_epoll_buffer.get_available_events()) {
            if(auto* connection = m_supervisor.get_connection(fd)) {
                process_connection(fd);
            } else if(fd == m_supervisor.get_pipe_fd()) {
                process_pipe_request();
            } else {
                spdlog::warn("Connection service epoll: unspecified file descriptor {}", fd);
            }
        }
    }

    void ConnectionService::process_connection(...) { }

    void ConnectionService::process_pipe_request() { }
}
