

#include "tds/ip/endpoint_v4.hpp"

#include "tds/ip/address_v4.hpp"
#include "tds/ip/ip_error.hpp"

#include <algorithm>
#include <cstring>

namespace tds::ip {
    EndpointV4::EndpointV4(AddressV4 address, Port port)
        : m_address{address}
        , m_port{port} { }

    AddressV4 EndpointV4::get_address() const {
        return m_address;
    }

    Port EndpointV4::get_port() const {
        return m_port;
    }

    std::ostream& operator<<(std::ostream& stream, EndpointV4 endpoint) {
        return stream << endpoint.get_address() << ':' << endpoint.get_port();
    }

    EndpointV4 make_endpoint_v4(const std::string& endpoint) {
        return make_endpoint_v4(endpoint.c_str());
    }

    EndpointV4 make_endpoint_v4(const char* endpoint) {
        const auto string_end = endpoint + std::strlen(endpoint);

        const auto colon_position = std::ranges::find(endpoint, string_end, ':');
        if(colon_position == string_end) {
            throw IpError{static_cast<int>(std::errc::invalid_argument), "expected ':' in endpoint"};
        }

        std::string address_part{endpoint, colon_position};
        const AddressV4 address = make_address_v4(address_part);
        const Port port = make_port(colon_position + 1);

        return EndpointV4{address, port};
    }

    std::string to_string(EndpointV4 endpoint) {
        return to_string(endpoint.get_address()) + ':' + to_string(endpoint.get_port());
    }
}
