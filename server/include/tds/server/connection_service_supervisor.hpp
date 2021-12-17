#pragma once

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/server/job.hpp"

#include <stop_token>
#include <thread>
#include <vector>

namespace tds::server {
    class ConnectionServiceSupervisor {
    public:
        ConnectionServiceSupervisor();

        ConnectionServiceSupervisor(const ConnectionServiceSupervisor&) = delete;
        ConnectionServiceSupervisor& operator=(const ConnectionServiceSupervisor&) = delete;

        void push_connection(...);

    private:
        linux::EpollDevice m_epoll;

        std::vector<std::jthread> m_jobs;
    };
}
