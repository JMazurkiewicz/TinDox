#include "tds/ip/endpoint_v4.hpp"

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
}
