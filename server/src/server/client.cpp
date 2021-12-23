#include "tds/server/client.hpp"

#include "tds/linux/linux_error.hpp"
#include "tds/server/server_logger.hpp"

namespace tds::server {
    Client::Client(ip::TcpSocket socket)
        : m_socket{std::move(socket)}
        , m_alive{true} { }

    ip::TcpSocket& Client::get_socket() noexcept {
        return m_socket;
    }

    bool Client::is_alive() {
        return m_alive;
    }

    void Client::handle() {
        read_requtests();
        write_responses();
        m_buffer.clear();
    }

    void Client::read_requtests() {
        ssize_t total_count = 0;
        ssize_t count;
        std::errc code = {};
        server_logger->set_level(spdlog::level::debug); // TOREMOVE

        do {
            std::array<char, 8> buffer;
            server_logger->debug("READING BYTEST...");
            count = m_socket.read(buffer.data(), buffer.size(), code);
            server_logger->debug("GOT {} BYTES...", count);

            if(count == -1) {
                break;
            } else {
                total_count += count;
                m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.begin() + count);
            }
        } while(code == std::errc{} && count != 0);

        server_logger->debug("ERRC = {}...", std::make_error_code(code).message());

        if(code != std::errc::resource_unavailable_try_again && code != std::errc{}) {
            m_alive = false;
        } else {
            std::string_view str{m_buffer.data(), m_buffer.size()};
            server_logger->debug("INTERNAL BUFFER SIZE: {}", m_buffer.size());
            server_logger->debug("FROM {} GOT: {}", m_socket.get_endpoint(), str);
        }
    }

    void Client::write_responses() try {
        ssize_t count = 0;

        do {
            count += m_socket.write(m_buffer.data() + count, m_buffer.size() - count);
        } while(count != m_buffer.size());
    } catch(const std::exception& e) {
        m_alive = false;
        throw;
    }
}
