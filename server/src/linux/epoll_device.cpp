#include "tds/linux/epoll_device.hpp"

#include "tds/linux/linux_error.hpp"

#include <algorithm>
#include <stdexcept>

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

    void EpollDevice::add_device(IoDevice& dev, EventType mode) {
        const int dev_fd = dev.get_fd();
        epoll_event event = {
            .events = static_cast<std::uint32_t>(mode),
            .data = {.fd = dev_fd},
        };

        if(epoll_ctl(get_fd(), EPOLL_CTL_ADD, dev_fd, &event) == -1) {
            throw LinuxError{"epoll_ctl(2)"};
        }
    }

    void EpollDevice::remove_device(const IoDevice& dev) {
        if(epoll_ctl(get_fd(), EPOLL_CTL_DEL, dev.get_fd(), nullptr) == -1) {
            throw LinuxError{"epoll_ctl(2)"};
        }
    }

    int EpollDevice::wait_for_events(EpollBuffer& buffer) {
        if(const int count = epoll_wait(get_fd(), buffer.data(), buffer.max_size(), m_timeout); count != -1) {
            buffer.set_size(count);
            return count;
        } else {
            throw LinuxError{"epoll_wait(2)"};
        }
    }
}
