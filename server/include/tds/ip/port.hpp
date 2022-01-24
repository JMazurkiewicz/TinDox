#pragma once

#include <cstdint>
#include <ostream>

namespace tds::ip {
    class Port {
    public:
        static const Port invalid;

        Port();
        explicit Port(std::uint16_t port);

        std::uint16_t as_integer() const;
        bool operator==(const Port&) const noexcept = default;

    private:
        std::uint16_t m_port;
    };

    std::ostream& operator<<(std::ostream& stream, Port port);
}
