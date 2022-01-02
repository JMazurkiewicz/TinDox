#include "tds/protocol/receiver.hpp"

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/linux_error.hpp"
#include "tds/protocol/response.hpp"

#include <stdexcept>

namespace tds::protocol {
    Receiver::Receiver(ip::TcpSocket& socket)
        : m_socket{socket}
        , m_errc{} { }

    std::span<const char> Receiver::receive() {
        m_errc = {};
        m_data.clear();

        read();
        check_error();
        return m_data;
    }

    void Receiver::read() {
        while(true) {
            std::array<char, 4096> buffer;
            const ssize_t count = m_socket.read(buffer.data(), buffer.size(), m_errc);
            if(m_errc != std::errc{} || count <= 0) {
                break;
            }

            m_data.insert(m_data.end(), buffer.begin(), buffer.begin() + count);
        }
    }

    void Receiver::check_error() {
        using enum std::errc;

        if(m_errc != resource_unavailable_try_again && m_errc != operation_would_block) {
            if(m_errc != std::errc{}) {
                throw linux::LinuxError{static_cast<int>(m_errc), "Receiver::read(2)"};
            } else if(m_data.size() == 0) {
                throw linux::LinuxError{ENOTCONN, "Receiver::read(2)"};
            }
        }
    }
}
