#pragma once

#include "tds/server/client_service_supervisor.hpp"

namespace tds::server {
    class ClientService {
    public:
        explicit ClientService(ClientServiceSupervisor& supervisor);

        ClientService(const ClientService&) = delete;
        ClientService& operator=(const ClientService&) = delete;

        void launch();

    private:
        void process_client_input(Client& client);
        void process_pipe_input();

        ClientServiceSupervisor* m_supervisor;
        bool m_running;
    };
}
