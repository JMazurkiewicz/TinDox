#pragma once

#include <cstdint>
#include <ostream>

namespace tds::ip {
    class AddressV4 {
    public:
        static const AddressV4 any;

        AddressV4();
        explicit AddressV4(std::uint32_t address);

        std::uint32_t as_integer() const;
        bool operator==(const AddressV4&) const noexcept = default;

    private:
        std::uint32_t m_address;
    };

    std::ostream& operator<<(std::ostream& stream, AddressV4 address);
}
