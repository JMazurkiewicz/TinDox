#pragma once

#include "tds/linux/io_device.hpp"

#include <unordered_set>
#include <vector>

#include <sys/epoll.h>

namespace tds::linux {
    class Epoll : public IoDevice {
    public:
        Epoll();

        void add_device(IoDevice& dev);
        void remove_device(IoDevice& dev);

        void handle() override;

    private:
        std::vector<int> m_fds;
    };
}
