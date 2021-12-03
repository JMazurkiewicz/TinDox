#pragma once

#include "tds/ip/address_v4.hpp"
#include "tds/ip/port.hpp"

namespace tds::ip {
    class EndpointV4 {
    public:
        EndpointV4() = default;
        explicit EndpointV4(AddressV4 address, Port port);

        AddressV4 get_address() const;
        Port get_port() const;

        constexpr bool operator==(const EndpointV4&) const noexcept = default;

    private:
        AddressV4 m_address;
        Port m_port;
    };

    std::ostream& operator<<(std::ostream& stream, EndpointV4 endpoint);

    EndpointV4 make_endpoint_v4(const std::string& endpoint);
    EndpointV4 make_endpoint_v4(const char* endpoint);

    std::string to_string(EndpointV4 endpoint);
}
