#pragma once

#include "tds/linux/io_device.hpp"

#include <span>
#include <unordered_map>

#include <sys/epoll.h>

namespace tds::linux {
    class EpollDevice : public IoDevice {
    public:
        EpollDevice();

        void set_timeout(int new_timeout);
        void add_device(IoDevice& dev);
        void handle() override;

    private:
        int wait_for_events(std::span<epoll_event> events);
        IoDevice& get_device(epoll_event event);

        int m_timeout;
        std::unordered_map<int, IoDevice*> m_devices;
    };
}
