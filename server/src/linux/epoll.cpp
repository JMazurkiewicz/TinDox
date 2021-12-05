#include "tds/linux/epoll.hpp"

#include "tds/linux/linux_error.hpp"

namespace tds::linux {
    Epoll::Epoll() {
        if(const int epoll_fd = epoll_create1(0)) {
            throw LinuxError{"epoll_create1"};
        } else {
            set_fd(epoll_fd);
        }
    }
}
