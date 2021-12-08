#pragma once

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/io_device.hpp"

#include <functional>

namespace tds::tcp {
    using ConnectionHandler = std::function<void(int connection_fd, ip::EndpointV4 client)>;

    class Listener : public linux::IoDevice {
    public:
        Listener();

        void set_backlog(int new_backlog);
        void set_connection_handler(ConnectionHandler connection_handler);

        void listen(ip::Port port);
        void listen(ip::EndpointV4 endpoint);

        void handle() override;

    private:
        int m_backlog;
        ConnectionHandler m_connection_handler;
    };
}
