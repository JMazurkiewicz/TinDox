#include "tds/server/user_connection.hpp"

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/io_device.hpp"

#include <spdlog/spdlog.h>

namespace tds::server {
    UserConnection::UserConnection(int fd, ip::EndpointV4 endpoint)
        : linux::IoDevice(fd)
        , m_endpoint{endpoint} { }

    UserConnection::~UserConnection() {
        spdlog::info("Closed connection {}", to_string(m_endpoint));
    }

    void UserConnection::handle() {
        try {
            std::array<char, 256> buffer;
            const auto amount = read(buffer.data(), buffer.size());
            if(amount == 0) {
                throw std::runtime_error{"Connection lost with " + to_string(m_endpoint)};
            }

            std::string_view msg{buffer.data(), static_cast<size_t>(amount)};
            spdlog::info("[{}]: {}", to_string(m_endpoint), msg);

            ssize_t written = 0;
            do {
                const int bytes = write(msg.data() + written, msg.size() - written);
                written += bytes;
            } while(written != msg.size());
        } catch(const std::exception& e) {
            spdlog::error(e.what());
            // parent->m_epoll.remove_device(*this);
            close();
            set_fd(-1);
        }
    }
}
