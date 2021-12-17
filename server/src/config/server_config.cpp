#include "tds/config/server_config.hpp"

#include "tds/config/defaults.hpp"

#include <stdexcept>

#include <fmt/core.h>

namespace tds::config {
    ServerConfig::ServerConfig()
        : m_max_thread_count{defaults::get_default_max_thread_count()}
        , m_max_user_count{defaults::get_default_max_user_count()}
        , m_backlog{defaults::get_default_backlog()}
        , m_port{defaults::get_default_port()} { }

    int ServerConfig::get_max_thread_count() const noexcept {
        return m_max_thread_count;
    }

    void ServerConfig::set_max_thread_count(int new_max_thread_count) {
        if(new_max_thread_count <= 1) {
            throw std::runtime_error{
                fmt::format("Max thread count should be at least 2 (is {})", new_max_thread_count)};
        } else {
            m_max_thread_count = new_max_thread_count;
        }
    }

    int ServerConfig::get_max_user_count() const noexcept {
        return m_max_user_count;
    }

    void ServerConfig::set_max_user_count(int new_max_user_count) {
        if(new_max_user_count < 1) {
            throw std::runtime_error{fmt::format("Max user count should be at least 1 (is {})", new_max_user_count)};
        } else {
            m_max_user_count = new_max_user_count;
        }
    }

    int ServerConfig::get_backlog() const noexcept {
        return m_backlog;
    }

    void ServerConfig::set_backlog(int new_backlog) {
        if(new_backlog < 1) {
            throw std::runtime_error{fmt::format("Backlog should be at least 1 (is {})", new_backlog)};
        } else {
            m_backlog = new_backlog;
        }
    }

    ip::Port ServerConfig::get_port() const noexcept {
        return m_port;
    }

    void ServerConfig::set_port(ip::Port new_port) {
        if(new_port == ip::Port::invalid) {
            throw std::runtime_error{fmt::format("{} is not valid port", ip::Port::invalid.as_integer())};
        } else {
            m_port = new_port;
        }
    }
}
