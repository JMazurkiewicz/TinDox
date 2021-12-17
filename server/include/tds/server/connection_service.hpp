#pragma once

#include "tds/server/connection_service.hpp"
#include "tds/server/connection_service_supervisor.hpp"

#include <stop_token>

namespace tds::server {
    class ConnectionService {
    public:
        explicit ConnectionService(std::stop_token& stop_token, ConnectionServiceSupervisor& supervisor);

        ConnectionService(const ConnectionService&) = delete;
        ConnectionService& operator=(const ConnectionService&) = delete;

    private:
        ConnectionServiceSupervisor& m_supervisor;
    };
}
