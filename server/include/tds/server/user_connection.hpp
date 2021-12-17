#pragma once

#include "tds/linux/io_device.hpp"

namespace tds::server {
    class UserConnection : public linux::IoDevice {
    public:
        explicit UserConnection();

        UserConnection(UserConnection&&) = default;
        UserConnection& operator=(UserConnection&&) = default;

    private:
    };
}
