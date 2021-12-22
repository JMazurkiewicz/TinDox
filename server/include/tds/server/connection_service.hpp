#pragma once

#include "tds/linux/epoll_buffer.hpp"
#include "tds/server/connection_service.hpp"
#include "tds/server/connection_service_supervisor.hpp"

#include <stop_token>

namespace tds::server {
    class ConnectionService {
    public:
        explicit ConnectionService(ConnectionServiceSupervisor& supervisor);

        ConnectionService(const ConnectionService&) = delete;
        ConnectionService& operator=(const ConnectionService&) = delete;

        void operator()();

    private:
        void wait_for_events();
        void process_events();

        void process_connection(...);
        void process_pipe_request();

        ConnectionServiceSupervisor& m_supervisor;
        linux::EpollBuffer m_epoll_buffer;
        bool m_running;
    };
}
