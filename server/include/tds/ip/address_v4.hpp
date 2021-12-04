#pragma once

#include <cstdint>
#include <ostream>
#include <span>
#include <string>

namespace tds::ip {
    class AddressV4 {
    public:
        static const AddressV4 any;
        static const AddressV4 broadcast;
        static const AddressV4 localhost;

        AddressV4();

        explicit AddressV4(std::uint32_t address);
        explicit AddressV4(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2, std::uint8_t b3);

        std::span<const std::byte, 4> as_bytes() const;
        std::uint32_t as_integer() const;

        bool operator==(const AddressV4&) const noexcept = default;

    private:
        std::uint32_t m_address;
    };

    std::ostream& operator<<(std::ostream& stream, AddressV4 address);

    AddressV4 make_address_v4(const std::string& address);
    AddressV4 make_address_v4(const char* address);

    std::string to_string(AddressV4 address);
}
