#include "tds/server/client_service_supervisor.hpp"

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/linux/pipe_device.hpp"
#include "tds/server/client_service.hpp"
#include "tds/server/server_logger.hpp"

#include <algorithm>
#include <ranges>
#include <system_error>

namespace tds::server {
    ClientServiceSupervisor::ClientServiceSupervisor(protocol::ServerContext& server_context)
        : m_running{true}
        , m_pipes{linux::make_pipe()}
        , m_clients{server_context} {
        m_epoll.add_device(m_pipes.m_read_device, linux::EventType::in);
    }

    ClientServiceSupervisor::~ClientServiceSupervisor() {
        stop();
    }

    void ClientServiceSupervisor::create_services(const config::ServerConfig& config) {
        for(int _ : std::views::iota(0, config.get_max_thread_count())) {
            m_jobs.emplace_back([&] {
                ClientService job{*this, config};
                job.launch();
            });
        }
    }

    void ClientServiceSupervisor::accept_connection(ip::TcpSocket connection) {
        const int socket_fd = connection.get_fd();
        const ip::EndpointV4 endpoint = connection.get_endpoint();

        try {
            m_epoll.add_device(connection,
                               linux::EventType::in | linux::EventType::edge_triggered | linux::EventType::one_shot);
            m_clients.spawn_client(std::move(connection));
        } catch(const std::system_error& e) {
            server_logger->error("ClientServiceSupervisor: failed to add connection from {} (fd = {}): {} ({})",
                                 endpoint, socket_fd, e.what(), e.code());
            m_clients.close_one(socket_fd);
        }
    }

    std::size_t ClientServiceSupervisor::get_client_count() {
        return m_clients.get_client_count();
    }

    bool ClientServiceSupervisor::has_client(int fd) {
        return m_clients.has_client(fd);
    }

    Client& ClientServiceSupervisor::get_client(int fd) {
        return m_clients.get_client(fd);
    }

    int ClientServiceSupervisor::get_pipe_fd() const noexcept {
        return m_pipes.m_read_device.get_fd();
    }

    void ClientServiceSupervisor::wait_for_events(linux::EpollBuffer& buffer) {
        m_epoll.wait_for_events(buffer);
    }

    void ClientServiceSupervisor::rearm_device(linux::IoDevice& device) {
        const int fd = device.get_fd();
        Client& client = m_clients.get_client(fd);

        if(client.is_alive()) {
            m_epoll.rearm_device(device, linux::EventType::edge_triggered | linux::EventType::one_shot |
                                             client.get_required_events());
        } else {
            m_clients.close_one(fd);
        }
    }

    void ClientServiceSupervisor::stop() {
        if(m_running) {
            server_logger->warn("ClientServiceSupervisor: stop requested");

            const char stop_signal = 'A';
            m_pipes.m_write_device.write(&stop_signal, 1);
            std::ranges::for_each(m_jobs, [](std::thread& t) { t.join(); });

            m_clients.close_all();
            m_running = false;
        }
    }
}
