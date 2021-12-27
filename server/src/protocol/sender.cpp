#include "tds/protocol/sender.hpp"

#include "tds/linux/linux_error.hpp"
#include "tds/protocol/response.hpp"

namespace tds::protocol {
    Sender::Sender()
        : m_socket{nullptr}
        , m_errc{}
        , m_sent_byte_count{0} { }

    void Sender::add_response(Response response) {
        m_responses.push(std::move(response));
    }

    bool Sender::has_responses() const noexcept {
        return !m_responses.empty();
    }

    void Sender::set_device(ip::TcpSocket& socket) {
        m_socket = &socket;
    }

    std::size_t Sender::send() {
        m_errc = {};
        m_sent_byte_count = 0;

        write();
        check_error();
        return m_sent_byte_count;
    }

    void Sender::write() {
        while(!m_responses.empty()) {
            Response& response = m_responses.front();
            const ssize_t count =
                m_socket->write(response.get_data_pointer(), response.get_remaining_byte_count(), m_errc);
            if(m_errc != std::errc{} || count <= 0) {
                break;
            }

            m_sent_byte_count += count;
            response.add_read_byte_count(count);
            if(response.is_fully_sent()) {
                m_responses.pop();
            }
        }
    }

    void Sender::check_error() {
        using enum std::errc;

        if(m_errc != std::errc{} && m_errc != resource_unavailable_try_again && m_errc != operation_would_block) {
            throw linux::LinuxError{static_cast<int>(m_errc), "Sender::write(2)"};
        }
    }
}
