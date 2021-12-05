#include "tds/linux/io_device.hpp"

#include "tds/linux/streambuf.hpp"

#include <unistd.h>

namespace tds::linux {
    IoDevice::IoDevice()
        : m_fd{-1} { }

    IoDevice::IoDevice(int fd)
        : m_fd{fd} { }

    IoDevice::~IoDevice() {
        raw_close();
    }

    int IoDevice::get_fd() const noexcept {
        return m_fd;
    }

    bool IoDevice::is_valid() const noexcept {
        return get_fd() >= 0;
    }

    void IoDevice::set_fd(int fd) {
        m_fd = fd;
    }

    void IoDevice::raw_close() {
        if(is_valid()) {
            close(m_fd);
        }
    }
}
