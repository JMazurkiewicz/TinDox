#pragma once

#include "tds/config/server_config.hpp"
#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/linux/io_device.hpp"
#include "tds/linux/pipe_device.hpp"
#include "tds/server/client.hpp"
#include "tds/server/client_pool.hpp"

#include <thread>
#include <vector>

namespace tds::server {
    class ClientServiceSupervisor {
    public:
        ClientServiceSupervisor();
        ~ClientServiceSupervisor();

        ClientServiceSupervisor(const ClientServiceSupervisor&) = delete;
        ClientServiceSupervisor& operator=(const ClientServiceSupervisor&) = delete;

        void create_services(const config::ServerConfig& config);
        void accept_connection(ip::TcpSocket connection);

        std::size_t get_client_count();
        bool has_client(int fd);
        Client& get_client(int fd);
        int get_pipe_fd() const noexcept;

        void wait_for_events(linux::EpollBuffer& buffer);
        void rearm_device(linux::IoDevice& device);

        void stop();

    private:
        bool m_running;
        linux::EpollDevice m_epoll;
        linux::PipeDevices m_pipes;
        ClientPool m_clients;
        std::vector<std::thread> m_jobs;
    };
}
