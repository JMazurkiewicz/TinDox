#include "tds/linux/epoll.hpp"

#include "tds/linux/linux_error.hpp"

#include <algorithm>
#include <ranges>

namespace tds::linux {
    Epoll::Epoll() {
        if(const int epoll_fd = epoll_create1(0); epoll_fd != -1) {
            set_fd(epoll_fd);
        } else {
            throw LinuxError{"epoll_create1(2)"};
        }
    }

    void Epoll::add_device(IoDevice& dev) {
        const int dev_fd = dev.get_fd();
        epoll_event event = {.events = EPOLLIN, .data = {.fd = dev_fd}};

        if(epoll_ctl(get_fd(), EPOLL_CTL_ADD, dev_fd, &event) == 0) {
            m_devices[dev_fd] = &dev;
        } else {
            throw LinuxError{"epoll_ctl(2)"};
        }
    }

    void Epoll::handle() {
        std::array<epoll_event, 256> events; // @todo temporary solution
        const int event_count = get_event_count(events);

        auto devices =
            events | std::views::take(event_count) | std::views::transform(std::bind_front(&Epoll::get_device, this));
        std::ranges::for_each(devices, &IoDevice::handle);
    }

    int Epoll::get_event_count(std::span<epoll_event> events) {
        if(const int count = epoll_wait(get_fd(), events.data(), events.size(), default_timeout.count()); count != -1) {
            return count;
        } else {
            throw LinuxError{"epoll_wait(2)"};
        }
    }

    IoDevice& Epoll::get_device(epoll_event event) {
        return *m_devices[event.data.fd];
    }
}
