#pragma once

#include "tds/config/server_config.hpp"
#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/pipe_device.hpp"
#include "tds/server/client.hpp"
#include "tds/server/client_pool.hpp"

#include <thread>
#include <vector>

namespace tds::server {
    class ClientServiceSupervisor {
    public:
        ClientServiceSupervisor();
        ClientServiceSupervisor(const ClientServiceSupervisor&) = delete;
        ClientServiceSupervisor& operator=(const ClientServiceSupervisor&) = delete;

        ~ClientServiceSupervisor();

        void set_config(const config::ServerConfig& config);
        void create_services();

        void add_connection(ip::TcpSocket connection);

        bool has_client(int fd) const;
        Client& get_client(int fd);
        int get_pipe_fd() const noexcept;
        void wait_for_events(linux::EpollBuffer& buffer);

        void stop();

    private:
        const config::ServerConfig* m_config;
        bool m_running;
        linux::EpollDevice m_epoll;
        linux::PipeDevices m_pipes;
        ClientPool m_clients;
        std::vector<std::thread> m_jobs;
    };
}
