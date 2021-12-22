#include "tds/ip/tcp_socket.hpp"

#include "tds/ip/endpoint_v4.hpp"

namespace tds::ip {
    TcpSocket::TcpSocket(int fd, EndpointV4 endpoint)
        : IoDevice(fd)
        , m_endpoint{endpoint} { }

    EndpointV4 TcpSocket::get_endpoint() const noexcept {
        return m_endpoint;
    }

    AddressV4 TcpSocket::get_address() const noexcept {
        return get_endpoint().get_address();
    }

    Port TcpSocket::get_port() const noexcept {
        return get_endpoint().get_port();
    }
}
