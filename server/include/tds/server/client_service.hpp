#pragma once

#include "tds/linux/epoll_buffer.hpp"
#include "tds/server/client_service_supervisor.hpp"

namespace tds::server {
    class ClientService {
    public:
        explicit ClientService(ClientServiceSupervisor& supervisor);

        ClientService(const ClientService&) = delete;
        ClientService& operator=(const ClientService&) = delete;
        ClientService(ClientService&&) noexcept = default;
        ClientService& operator=(ClientService&&) noexcept = default;

        void operator()();

    private:
        void wait_for_events();
        void process_events();
        void process_pipe_request();

        ClientServiceSupervisor* m_supervisor;
        linux::EpollBuffer m_epoll_buffer;
        bool m_running;
    };
}
