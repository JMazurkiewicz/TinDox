#include "tds/ip/port.hpp"

#include <array>
#include <charconv>

#include <arpa/inet.h>

namespace tds::ip {
    const Port Port::invalid{0};

    Port::Port()
        : m_port{0} { }

    Port::Port(std::uint16_t port)
        : m_port{htons(port)} { }

    std::uint16_t Port::as_integer() const {
        return ntohs(m_port);
    }

    std::ostream& operator<<(std::ostream& stream, Port port) {
        std::array<char, 6> buffer;
        const auto [end_ptr, errc] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), port.as_integer());
        if(errc != std::errc{}) {
            stream.setstate(std::ios_base::failbit);
        }
        return stream << std::string_view{buffer.data(), end_ptr};
    }
}
