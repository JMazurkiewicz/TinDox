#pragma once

#include "tds/config/server_config.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/server/client_service_supervisor.hpp"

namespace tds::server {
    class ClientService {
    public:
        explicit ClientService(ClientServiceSupervisor& supervisor, const config::ServerConfig& config);

        ClientService(const ClientService&) = delete;
        ClientService& operator=(const ClientService&) = delete;

        void launch();

    private:
        void main_loop();
        void process_client_input(Client& client, linux::EventType events);
        void process_pipe_input();

        ClientServiceSupervisor& m_supervisor;
        linux::EpollBuffer m_epoll_buffer;
        bool m_running : 1;
    };
}
