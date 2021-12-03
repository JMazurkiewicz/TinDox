#pragma once

#include <cstdint>
#include <ostream>
#include <span>

namespace tds::ip {
    class Port {
    public:
        static const Port invalid;
        static const Port default_port;

        Port();

        explicit Port(std::uint16_t port);
        explicit Port(std::uint8_t b0, std::uint8_t b1);

        std::span<const std::byte, 2> as_bytes() const;
        std::uint16_t as_integer() const;

        bool operator==(const Port&) const = default;
        bool operator==(std::uint16_t port) const noexcept;

    private:
        std::uint16_t m_port;
    };

    std::ostream& operator<<(std::ostream& stream, Port port);

    Port make_port(const std::string& port);
    Port make_port(const char* port);

    std::string to_string(Port port);
}
