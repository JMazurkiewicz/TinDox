#pragma once

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/io_device.hpp"

#include <functional>

namespace tds::ip {
    using ConnectionHandler = std::function<void(int connection_fd, EndpointV4 client)>;

    class TcpListener : public linux::IoDevice {
    public:
        TcpListener();

        void set_backlog(int new_backlog);
        void set_connection_handler(ConnectionHandler connection_handler);

        void listen(Port port);
        void listen(EndpointV4 endpoint);

        void handle() override;

    private:
        int m_backlog;
        ConnectionHandler m_connection_handler;
    };
}
