#include "tds/ip/address_v4.hpp"

#include "tds/ip/ip_error.hpp"

#include <cstring>
#include <iostream>
#include <ranges>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace tds::ip {
    namespace {
        const char* address_to_cstring(AddressV4 address) {
            const in_addr addr = {.s_addr = htonl(address.as_integer())};
            return ::inet_ntoa(addr);
        }
    }

    const AddressV4 AddressV4::any{INADDR_ANY};
    const AddressV4 AddressV4::localhost{INADDR_LOOPBACK};
    const AddressV4 AddressV4::broadcast{INADDR_BROADCAST};

    AddressV4::AddressV4()
        : m_address{0} { }

    AddressV4::AddressV4(std::uint32_t address)
        : m_address{htonl(address)} { }

    AddressV4::AddressV4(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2, std::uint8_t b3)
        : AddressV4(static_cast<unsigned>(b0 << 24) | (b1 << 16) | (b2 << 8) | b3) { }

    std::span<const std::byte, 4> AddressV4::as_bytes() const {
        std::span<const std::uint32_t, 1> s{&m_address, 1};
        return std::as_bytes(s);
    }

    std::uint32_t AddressV4::as_integer() const {
        return ntohl(m_address);
    }

    std::ostream& operator<<(std::ostream& stream, AddressV4 address) {
        return stream << address_to_cstring(address);
    }

    AddressV4 make_address_v4(const std::string& address) {
        return make_address_v4(address.c_str());
    }

    AddressV4 make_address_v4(const char* address) {
        if(std::strcmp(address, "255.255.255.255") == 0) {
            return AddressV4::broadcast;
        } else if(std::strcmp(address, "0.0.0.0") == 0) {
            return AddressV4::any;
        } else {
            in_addr_t ip = inet_addr(address);

            if(ip == INADDR_NONE) {
                const addrinfo hints = {.ai_family = AF_INET};
                addrinfo* info = nullptr;

                if(auto errc = getaddrinfo(address, nullptr, &hints, &info); errc == 0) {
                    ip = reinterpret_cast<sockaddr_in*>(info->ai_addr)->sin_addr.s_addr;
                    freeaddrinfo(info);
                } else {
                    throw IpError{errc, get_gai_category(), "tds::ip::make_address_v4 failed"};
                }
            }

            return AddressV4{ntohl(ip)};
        }
    }

    std::string to_string(AddressV4 address) {
        return address_to_cstring(address);
    }
}
