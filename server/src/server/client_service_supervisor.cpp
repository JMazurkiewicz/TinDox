#include "tds/server/client_service_supervisor.hpp"

#include "tds/server/client_service.hpp"

#include <algorithm>
#include <ranges>

namespace tds::server {
    ClientServiceSupervisor::ClientServiceSupervisor()
        : m_read_pipe{-1} { }

    ClientServiceSupervisor::~ClientServiceSupervisor() {
        std::ranges::for_each(m_jobs, [](std::thread& t) { t.join(); });
    }

    void ClientServiceSupervisor::set_config(const config::ServerConfig& config) {
        m_config = &config;
    }

    void ClientServiceSupervisor::set_read_pipe(linux::PipeReadDevice read_pipe) {
        m_read_pipe = std::move(read_pipe);
    }

    void ClientServiceSupervisor::create_services() {
        for(int _ : std::views::iota(0, m_config->get_max_thread_count())) {
            m_jobs.emplace_back(ClientService{*this});
        }
    }

    void ClientServiceSupervisor::add_connection(ip::TcpSocket connection) {
        m_clients.add_client(std::move(connection));
    }

    bool ClientServiceSupervisor::has_client(int fd) const {
        return m_clients.has_client(fd);
    }

    Client& ClientServiceSupervisor::get_client(int fd) {
        return m_clients.get_client(fd);
    }

    int ClientServiceSupervisor::get_pipe_fd() const noexcept {
        return m_read_pipe.get_fd();
    }
}
