#include "tds/ip/tcp_socket.hpp"

#include "tds/linux/linux_error.hpp"

#include <netinet/in.h>
#include <netinet/tcp.h>

namespace tds::ip {
    TcpSocket::TcpSocket()
        : IoDevice(-1) { }

    TcpSocket::TcpSocket(int fd, EndpointV4 endpoint)
        : IoDevice(fd)
        , m_endpoint{endpoint} {
        const int flag = 1;
        if(setsockopt(get_fd(), IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) == -1) {
            throw linux::LinuxError{"setsockopt(2)"};
        }
    }

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
