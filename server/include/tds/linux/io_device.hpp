#pragma once

#include <span>

#include <unistd.h>

namespace tds::linux {
    class IoDevice {
    public:
        IoDevice();

        IoDevice(const IoDevice&) = delete;
        IoDevice& operator=(const IoDevice&) = delete;

        ~IoDevice();
        void close();

        int get_fd() const noexcept;
        bool is_valid() const noexcept;
        explicit operator bool() const noexcept;

        int release() noexcept;

        ssize_t read(std::span<std::byte> buffer);
        ssize_t read(void* buffer, std::size_t size);

        ssize_t write(std::span<const std::byte> buffer);
        ssize_t write(const void* buffer, std::size_t size);

        bool operator==(const IoDevice& other) const noexcept;

    protected:
        explicit IoDevice(int fd);

        IoDevice(IoDevice&&);
        IoDevice& operator=(IoDevice&&);

        void set_fd(int fd);

    private:
        int m_fd;
    };
}
