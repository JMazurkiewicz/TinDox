#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/protocol/response.hpp"

#include <queue>

namespace tds::protocol {
    class Sender {
    public:
        explicit Sender(ip::TcpSocket& socket);
        Sender(const Sender&) = delete;
        Sender& operator=(const Sender&) = delete;

        void add_response(Response response);
        bool has_responses() const noexcept;
        std::size_t send();

    private:
        void write();
        void check_error();

        ip::TcpSocket& m_socket;
        std::queue<Response> m_responses;
        std::errc m_errc;
        std::size_t m_sent_byte_count;
    };
}
