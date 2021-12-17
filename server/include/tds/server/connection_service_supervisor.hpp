#pragma once

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/pipe_device.hpp"
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

        void create_slaves();

        void set_read_pipe(linux::PipeReadDevice read_pipe);
        void push_connection(...);

        void* get_connection(int fd);
        int get_pipe_fd() const noexcept;

    private:
        linux::EpollDevice m_connection_epoll;
        linux::PipeReadDevice m_read_pipe;
        std::vector<std::thread> m_jobs;
    };
}
