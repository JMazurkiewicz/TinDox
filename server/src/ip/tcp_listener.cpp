#include "tds/ip/tcp_listener.hpp"

#include "tds/ip/address_v4.hpp"
#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/linux_error.hpp"

#include <cstring>
#include <stdexcept>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace tds::ip {
    TcpListener::TcpListener()
        : m_backlog{32}
        , m_connection_type{SocketType::blocking} {
        if(const int socket_fd = socket(AF_INET, SOCK_STREAM, 0); socket_fd == -1) {
            throw linux::LinuxError{"socket(2)"};
        } else {
            set_fd(socket_fd);
        }

        const int flag = 1;
        setsockopt(get_fd(), SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    }

    void TcpListener::set_backlog(int new_backlog) {
        if(new_backlog <= 0) {
            throw std::logic_error{"Listener: backlog should not be negative"};
        } else {
            m_backlog = new_backlog;
        }
    }

    void TcpListener::set_connection_handler(ConnectionHandler connection_handler) {
        m_connection_handler = std::move(connection_handler);
    }

    void TcpListener::set_connection_type(SocketType connection_type) {
        m_connection_type = connection_type;
    }

    void TcpListener::listen(Port port) {
        listen(EndpointV4{AddressV4::any, port});
    }

    void TcpListener::listen(EndpointV4 endpoint) {
        const sockaddr_in addr = {
            .sin_family = AF_INET,
            .sin_port = htons(endpoint.get_port().as_integer()),
            .sin_addr = {.s_addr = htonl(endpoint.get_address().as_integer())},
        };

        if(::bind(get_fd(), reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in)) == -1) {
            throw linux::LinuxError{"bind(2)"};
        }

        if(::listen(get_fd(), m_backlog) == -1) {
            throw linux::LinuxError{"listen(2)"};
        }
    }

    void TcpListener::handle_connection() {
        sockaddr_in addrbuf = {};
        socklen_t addr_length = sizeof(sockaddr_in);
        const int connection_fd =
            accept4(get_fd(), reinterpret_cast<sockaddr*>(&addrbuf), &addr_length, static_cast<int>(m_connection_type));

        if(connection_fd == -1) {
            throw linux::LinuxError{"accept4(2)"};
        } else if(addr_length != sizeof(sockaddr_in)) {
            ::close(connection_fd);
            throw std::runtime_error{"Listener: accept(2) returned invalid addrlen"};
        } else {
            auto addr = reinterpret_cast<const sockaddr_in*>(&addrbuf);
            if(addr->sin_family != AF_INET) {
                std::runtime_error{"Listener: received non-IPv4 connection"};
            }

            const AddressV4 address{ntohl(addr->sin_addr.s_addr)};
            const Port port{ntohs(addr->sin_port)};
            const EndpointV4 endpoint{address, port};

            m_connection_handler(TcpSocket{connection_fd, endpoint});
        }
    }
}
