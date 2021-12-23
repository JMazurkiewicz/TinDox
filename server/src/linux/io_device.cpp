#include "tds/linux/io_device.hpp"

#include "tds/linux/linux_error.hpp"

#include <system_error>
#include <utility>

#include <unistd.h>

namespace tds::linux {
    IoDevice::IoDevice()
        : m_fd{-1} { }

    IoDevice::~IoDevice() {
        ::close(m_fd);
    }

    void IoDevice::close() {
        ::close(std::exchange(m_fd, -1));
    }

    int IoDevice::get_fd() const noexcept {
        return m_fd;
    }

    bool IoDevice::is_valid() const noexcept {
        return get_fd() >= 0;
    }

    IoDevice::operator bool() const noexcept {
        return is_valid();
    }

    int IoDevice::release() noexcept {
        return std::exchange(m_fd, -1);
    }

    ssize_t IoDevice::read(void* buffer, std::size_t size) {
        std::errc code = {};
        const ssize_t count = read(buffer, size, code);
        if(code != std::errc{}) {
            throw LinuxError{"read(2)"};
        } else {
            return count;
        }
    }

    ssize_t IoDevice::read(void* buffer, std::size_t size, std::errc& code) noexcept {
        const ssize_t count = ::read(get_fd(), buffer, size);
        if(count == -1) {
            code = static_cast<std::errc>(errno);
        }
        return count;
    }

    ssize_t IoDevice::write(const void* buffer, std::size_t size) {
        std::errc code = {};
        const ssize_t count = write(buffer, size, code);
        if(code != std::errc{}) {
            throw LinuxError{"write(2)"};
        } else {
            return count;
        }
    }

    ssize_t IoDevice::write(const void* buffer, std::size_t size, std::errc& code) noexcept {
        const ssize_t count = ::write(get_fd(), buffer, size);
        if(count == -1) {
            code = static_cast<std::errc>(errno);
        }
        return count;
    }

    bool IoDevice::operator==(const IoDevice& other) const noexcept {
        return get_fd() == other.get_fd();
    }

    IoDevice::IoDevice(int fd)
        : m_fd{fd} { }

    IoDevice::IoDevice(IoDevice&& other)
        : m_fd{std::exchange(other.m_fd, -1)} { }

    IoDevice& IoDevice::operator=(IoDevice&& other) {
        if(this != &other) {
            m_fd = std::exchange(other.m_fd, -1);
        }
        return *this;
    }

    void IoDevice::set_fd(int fd) {
        m_fd = fd;
    }
}
