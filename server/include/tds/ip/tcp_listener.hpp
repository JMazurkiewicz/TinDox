#pragma once

#include "tds/ip/endpoint_v4.hpp"
#include "tds/ip/socket_type.hpp"
#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/io_device.hpp"

#include <functional>

namespace tds::ip {
    class TcpListener : public linux::IoDevice {
    public:
        using ConnectionHandler = std::function<void(TcpSocket)>;

        TcpListener();

        void set_backlog(int new_backlog);
        void set_connection_handler(ConnectionHandler connection_handler);
        void set_connection_type(SocketType connection_type);

        void listen(Port port);
        void listen(EndpointV4 endpoint);

        void handle_connection();

    private:
        int m_backlog;
        SocketType m_connection_type;
        ConnectionHandler m_connection_handler;
    };
}
