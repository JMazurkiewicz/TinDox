#pragma once

#include "tds/ip/port.hpp"

namespace tds::config {
    class ServerConfig {
    public:
        ServerConfig();

        int get_max_thread_count() const noexcept;
        void set_max_thread_count(int new_max_thread_count);

        int get_max_user_count() const noexcept;
        void set_max_user_count(int new_max_user_count);

        int get_backlog() const noexcept;
        void set_backlog(int new_backlog);

        ip::Port get_port() const noexcept;
        void set_port(ip::Port new_port);

    private:
        int m_max_thread_count;
        int m_max_user_count;
        int m_backlog;
        ip::Port m_port;
    };
}
