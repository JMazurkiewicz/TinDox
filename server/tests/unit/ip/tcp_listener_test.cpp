#include <catch2/catch_test_macros.hpp>

#include "tds/ip/address_v4.hpp"
#include "tds/ip/tcp_listener.hpp"
#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/linux_error.hpp"

#include <thread>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace tds::ip;
using namespace std::chrono_literals;

TEST_CASE("tds::ip::TcpListener", "[ip]") {
    bool connection_established = false;
    bool server_error = false;
    bool client_error = false;

    const Port server_port{44444};

    std::thread server{[&] {
        try {
            TcpListener tcp;
            tcp.set_connection_handler([&](int fd, EndpointV4 endpoint) {
                connection_established = (fd >= 0) && (endpoint.get_address() == AddressV4::localhost);
                close(fd);
            });
            tcp.listen(server_port);
            tcp.handle_last_connection();
        } catch(...) {
            server_error = true;
        }
    }};

    std::this_thread::sleep_for(2s);

    std::thread client{[&] {
        try {
            if(const int fd = socket(AF_INET, SOCK_STREAM, 0); fd == -1) {
                client_error = true;
            } else {
                const sockaddr_in addr = {
                    .sin_family = AF_INET,
                    .sin_port = htons(server_port.as_integer()),
                    .sin_addr = {.s_addr = htonl(AddressV4::any.as_integer())},
                };

                if(connect(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in)) == -1) {
                    client_error = true;
                } else {
                    close(fd);
                }
            }
        } catch(...) {
            client_error = true;
        }
    }};

    REQUIRE(client.joinable());
    REQUIRE(server.joinable());

    client.join();
    server.join();

    REQUIRE(!client_error);
    REQUIRE(!server_error);
    REQUIRE(connection_established);
}

TEST_CASE("tds::ip::{TcpListener+EpollDevice}", "[ip]") {
    bool epoll_handled = false;
    bool connection_established = false;
    bool server_error = false;
    bool client_error = false;

    const Port server_port{55555};

    std::thread server{[&] {
        try {
            TcpListener tcp;
            tcp.set_connection_handler([&](int fd, EndpointV4 endpoint) {
                epoll_handled = true;
                connection_established = (fd >= 0) && (endpoint.get_address() == AddressV4::localhost);
                close(fd);
            });
            tcp.listen(server_port);

            tds::linux::EpollDevice epoll_device;
            epoll_device.add_device(tcp);

            tds::linux::EpollBuffer buffer{16};
            epoll_device.wait_for_events(buffer);

            for(int fd : buffer.get_available_events()) {
                if(fd == tcp.get_fd()) {
                    tcp.handle_last_connection();
                }
            }

        } catch(std::exception& e) {
            server_error = true;
        }
    }};

    std::this_thread::sleep_for(2s);

    std::thread client{[&] {
        try {
            if(const int fd = socket(AF_INET, SOCK_STREAM, 0); fd == -1) {
                client_error = true;
            } else {
                const sockaddr_in addr = {
                    .sin_family = AF_INET,
                    .sin_port = htons(server_port.as_integer()),
                    .sin_addr = {.s_addr = htonl(AddressV4::any.as_integer())},
                };

                if(connect(fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in)) == -1) {
                    client_error = true;
                } else {
                    close(fd);
                }
            }
        } catch(...) {
            client_error = true;
        }
    }};

    REQUIRE(client.joinable());
    REQUIRE(server.joinable());

    client.join();
    server.join();

    REQUIRE(!client_error);
    REQUIRE(!server_error);
    REQUIRE(connection_established);
    REQUIRE(epoll_handled);
}
