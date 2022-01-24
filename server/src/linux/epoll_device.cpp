#include "tds/linux/epoll_device.hpp"

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/linux_error.hpp"

#include <algorithm>

namespace tds::linux {
    EpollDevice::EpollDevice()
        : m_timeout{-1} {
        if(const int epoll_fd = epoll_create1(0); epoll_fd != -1) {
            set_fd(epoll_fd);
        } else {
            throw LinuxError{"epoll_create1(2)"};
        }
    }

    void EpollDevice::set_timeout(int new_timeout) {
        m_timeout = std::ranges::max(new_timeout, -1);
    }

    void EpollDevice::add_device(IoDevice& dev, EventType events) {
        epoll_ctl(dev.get_fd(), EPOLL_CTL_ADD, events);
    }

    void EpollDevice::rearm_device(IoDevice& dev, EventType events) {
        epoll_ctl(dev.get_fd(), EPOLL_CTL_MOD, events);
    }

    int EpollDevice::wait_for_events(EpollBuffer& buffer) {
        if(const int count = epoll_wait(get_fd(), buffer.data(), buffer.max_size(), m_timeout); count != -1) {
            buffer.set_size(count);
            return count;
        } else {
            throw LinuxError{"epoll_wait(2)"};
        }
    }

    void EpollDevice::epoll_ctl(int fd, int operation, EventType events) {
        epoll_event event = {
            .events = static_cast<std::uint32_t>(events),
            .data = {.fd = fd},
        };

        if(::epoll_ctl(get_fd(), operation, fd, &event) == -1) {
            throw LinuxError{"epoll_ctl(2)"};
        }
    }
}
