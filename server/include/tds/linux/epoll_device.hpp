#pragma once

#include "tds/linux/event_type.hpp"
#include "tds/linux/io_device.hpp"

namespace tds::linux {
    class EpollBuffer;

    class EpollDevice : public IoDevice {
    public:
        struct Event;

        EpollDevice();

        void set_timeout(int new_timeout);

        void add_device(IoDevice& dev, EventType events);
        void rearm_device(IoDevice& dev, EventType events);
        int wait_for_events(EpollBuffer& buffer);

    private:
        void epoll_ctl(int fd, int operation, EventType events);

        int m_timeout;
    };

    struct EpollDevice::Event {
        int fd;
        EventType events;
    };
}
