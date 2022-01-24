#include "tds/ip/address_v4.hpp"

#include <arpa/inet.h>

namespace tds::ip {
    const AddressV4 AddressV4::any{INADDR_ANY};

    AddressV4::AddressV4()
        : m_address{0} { }

    AddressV4::AddressV4(std::uint32_t address)
        : m_address{htonl(address)} { }

    std::uint32_t AddressV4::as_integer() const {
        return ntohl(m_address);
    }

    std::ostream& operator<<(std::ostream& stream, AddressV4 address) {
        const in_addr addr = {.s_addr = htonl(address.as_integer())};
        return stream << inet_ntoa(addr);
    }
}
