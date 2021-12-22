#pragma once

#include "tds/ip/endpoint_v4.hpp"
#include "tds/linux/io_device.hpp"

namespace tds::server {
    class UserConnection : public linux::IoDevice {
    public:
        explicit UserConnection(int fd, ip::EndpointV4 endpoint);
        ~UserConnection();

        UserConnection(UserConnection&&) = default;
        UserConnection& operator=(UserConnection&&) = default;

        void handle();

    private:
        ip::EndpointV4 m_endpoint;
    };
}
