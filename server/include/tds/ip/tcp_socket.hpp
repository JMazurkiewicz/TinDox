#pragma once

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/io_device.hpp"

namespace tds::ip {
    class TcpSocket : public linux::IoDevice {
    public:
        TcpSocket();
        explicit TcpSocket(int fd, EndpointV4 endpoint);

        TcpSocket(TcpSocket&&) = default;
        TcpSocket& operator=(TcpSocket&&) = default;

        EndpointV4 get_endpoint() const noexcept;

    private:
        EndpointV4 m_endpoint;
    };
}
