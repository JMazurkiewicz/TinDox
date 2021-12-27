#include "tds/server/client_service_supervisor.hpp"

#include "tds/linux/event_type.hpp"
#include "tds/linux/pipe_device.hpp"
#include "tds/server/client_service.hpp"
#include "tds/server/server_logger.hpp"

#include <algorithm>
#include <ranges>

namespace tds::server {
    ClientServiceSupervisor::ClientServiceSupervisor()
        : m_running{true}
        , m_pipes{linux::make_pipe(true)} {
        m_epoll.add_device(m_pipes.m_read_device);
    }

    ClientServiceSupervisor::~ClientServiceSupervisor() {
        stop();
    }

    void ClientServiceSupervisor::create_services(const config::ServerConfig& config) {
        for(int _ : std::views::iota(0, config.get_max_thread_count())) {
            m_jobs.emplace_back([this] {
                ClientService job{*this};
                job.launch();
            });
        }
    }

    void ClientServiceSupervisor::accept_connection(ip::TcpSocket connection) {
        m_epoll.add_device(connection,
                           linux::EventType::in | linux::EventType::edge_triggered | linux::EventType::one_shot);
        m_clients.add_client(std::move(connection));
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
            server_logger->warn("Client supervisor: stop requested");

            const char stop_signal = 'A';
            m_pipes.m_write_device.write(&stop_signal, 1);
            std::ranges::for_each(m_jobs, [](std::thread& t) { t.join(); });

            m_clients.close_all();
            m_running = false;
        }
    }
}
