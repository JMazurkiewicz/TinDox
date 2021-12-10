#pragma once

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/io_device.hpp"

namespace tds::linux {
    enum class EpollMode : std::uint32_t {
        level_triggered = EPOLLIN,
        edge_triggered = EPOLLIN | EPOLLET,
    };

    class EpollDevice : public IoDevice {
    public:
        EpollDevice();

        void set_timeout(int new_timeout);

        void add_device(IoDevice& dev, EpollMode mode = EpollMode::level_triggered);
        void remove_device(const IoDevice& dev);
        int wait_for_events(EpollBuffer& buffer);

    private:
        int m_timeout;
    };
}
