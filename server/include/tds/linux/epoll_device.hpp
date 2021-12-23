#pragma once

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/io_device.hpp"

namespace tds::linux {
    class EpollDevice : public IoDevice {
    public:
        EpollDevice();

        void set_timeout(int new_timeout);

        void add_device(IoDevice& dev, EventType events = EventType::in);
        void rearm_device(IoDevice& dev, EventType events = EventType::in);
        void remove_device(const IoDevice& dev);
        int wait_for_events(EpollBuffer& buffer);

    private:
        void epoll_ctl(int fd, int op, EventType events);

        int m_timeout;
    };
}
