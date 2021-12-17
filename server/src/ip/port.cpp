#include "tds/ip/port.hpp"

#include "tds/ip/ip_error.hpp"

#include <array>
#include <charconv>
#include <cstring>

#include <arpa/inet.h>

namespace tds::ip {
    namespace {
        const char* port_to_cstring(Port port) {
            static thread_local std::array<char, 6> buffer;
            const auto [end_ptr, errc] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), port.as_integer());

            if(errc != std::errc{}) {
                throw IpError{static_cast<int>(errc), "port_to_cstring failed"};
            } else {
                *end_ptr = '\0';
                return buffer.data();
            }
        }
    }

    const Port Port::invalid{0};

    Port::Port()
        : m_port{0} { }

    Port::Port(std::uint16_t port)
        : m_port{htons(port)} { }

    Port::Port(std::uint8_t b0, std::uint8_t b1)
        : Port(static_cast<unsigned>(b0 << 8) | b1) { }

    std::span<const std::byte, 2> Port::as_bytes() const {
        const std::span<const std::uint16_t, 1> s{&m_port, 1};
        return std::as_bytes(s);
    }

    std::uint16_t Port::as_integer() const {
        return ntohs(m_port);
    }

    bool Port::operator==(std::uint16_t port) const noexcept {
        return as_integer() == port;
    }

    std::ostream& operator<<(std::ostream& stream, Port port) {
        return stream << port_to_cstring(port);
    }

    Port make_port(const std::string& port) {
        return make_port(port.c_str());
    }

    Port make_port(const char* port) {
        std::uint16_t result = 0;
        const char* const port_end = port + std::strlen(port);
        const auto [ptr, errc] = std::from_chars(port, port_end, result);

        if(errc != std::errc{}) {
            throw IpError{static_cast<int>(errc), "make_port failed"};
        } else if(ptr != port_end) {
            throw IpError{static_cast<int>(std::errc::invalid_argument),
                          "make_port failed (invalid characters at the end of string)"};
        } else {
            return Port{result};
        }
    }

    std::string to_string(Port port) {
        return port_to_cstring(port);
    }
}
