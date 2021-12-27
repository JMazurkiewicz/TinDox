#include <catch2/catch_test_macros.hpp>

#include "tds/ip/tcp_listener.hpp"
#include "tds/ip/tcp_socket.hpp"
#include "tds/protocol/receiver.hpp"

#include <algorithm>
#include <thread>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std::literals;
using namespace tds;
using namespace protocol;

TEST_CASE("tds::protocol::Receiver", "[protocol]") {
    const ip::Port server_port{46789};
    ip::TcpListener listener;
    ip::TcpSocket socket;

    listener.set_connection_type(ip::SocketType::nonblocking);
    listener.set_connection_handler([&](ip::TcpSocket s) {
        REQUIRE(s.is_valid());
        socket = std::move(s);
    });
    listener.listen(server_port);

    std::thread temporary_listener{[&] {
        listener.handle_connection();
    }};
    std::this_thread::sleep_for(2s);

    int client_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    REQUIRE(client_fd != -1);

    const sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(server_port.as_integer()),
        .sin_addr = {.s_addr = htonl(ip::AddressV4::any.as_integer())},
    };
    REQUIRE(connect(client_fd, reinterpret_cast<const sockaddr*>(&addr), sizeof(sockaddr_in)) != -1);

    std::this_thread::sleep_for(2s);
    REQUIRE(socket.is_valid());
    temporary_listener.join();

    const std::string_view message =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque vulputate augue quis pulvinar faucibus. In "
        "hac habitasse platea dictumst. Mauris vel nibh vulputate, eleifend quam at, aliquam lorem. Maecenas augue mi, "
        "sollicitudin et justo ac, aliquet condimentum elit. Vivamus laoreet bibendum massa id accumsan. In eget "
        "turpis porta, sodales diam nec, eleifend velit. Phasellus rhoncus nec turpis interdum congue. Fusce ac lorem "
        "commodo, dictum diam in, suscipit justo. Proin in dictum magna. Aliquam erat volutpat."
        "Nunc consectetur vitae leo id pretium. Fusce in dapibus leo. Nullam at tristique ante, non commodo enim. Duis "
        "et finibus nunc. Curabitur ut ultricies urna. Duis at ligula massa. Proin eu egestas dui. Vivamus imperdiet "
        "ex quis venenatis hendrerit. Donec viverra condimentum neque, vel congue mauris pharetra vel. Quisque "
        "bibendum faucibus mi, a tristique sem mollis ac."
        "Ut auctor turpis in varius sagittis. Duis dapibus volutpat maximus. Aenean mollis placerat leo in vehicula. "
        "Duis mattis maximus velit, in efficitur magna aliquam ac. Donec eleifend pretium magna, id porttitor elit "
        "interdum non. Duis molestie, quam non rutrum efficitur, magna libero mollis ipsum, ac pellentesque enim "
        "turpis quis leo. Nam rutrum libero eu mauris efficitur aliquam non vel metus. Donec nulla justo, molestie vel "
        "purus rhoncus, lobortis pretium felis. Quisque tellus lacus, sodales quis congue vitae, vestibulum ut sem. "
        "Cras quam dolor, gravida ut mauris et, sollicitudin vestibulum velit. Aliquam erat volutpat. Suspendisse "
        "ornare ex ac tempor luctus. Sed eu varius velit."
        "Morbi vitae vehicula metus, quis convallis eros. Aenean a pretium orci. Nam vestibulum sem et libero sodales, "
        "vitae fermentum leo fringilla. Vivamus rutrum tortor et dui suscipit mattis. Cras iaculis a augue sit amet "
        "ultricies. Ut fringilla sodales quam, et egestas lacus aliquam a. Donec tempus dignissim dui, vel euismod "
        "ligula facilisis vitae. Phasellus et lorem orci. Nam tincidunt lectus sem, eget dictum ante faucibus mollis. "
        "Duis et elementum diam. Suspendisse ut sed.";
    write(client_fd, message.data(), message.size());

    Receiver receiver;
    receiver.set_device(socket);
    const std::span<const char> result = receiver.receive();

    REQUIRE(!result.empty());
    REQUIRE(std::ranges::equal(result, message));
    close(client_fd);
}
