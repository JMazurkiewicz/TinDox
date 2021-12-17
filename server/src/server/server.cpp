#include "tds/server/server.hpp"

namespace tds::server {
    Server::Server(std::filesystem::path root)
        : m_root{std::move(root)} { }

    void Server::set_config(const config::ServerConfig& config) {
        m_config = config;
    }

    void Server::launch() {

    }
}
