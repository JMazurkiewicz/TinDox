#pragma once

#include <system_error>

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

        ssize_t read(void* buffer, std::size_t size);
        ssize_t read(void* buffer, std::size_t size, std::errc& code) noexcept;

        ssize_t write(const void* buffer, std::size_t size);
        ssize_t write(const void* buffer, std::size_t size, std::errc& code) noexcept;

    protected:
        explicit IoDevice(int fd);

        IoDevice(IoDevice&&);
        IoDevice& operator=(IoDevice&&);

        void set_fd(int fd);

    private:
        int m_fd;
    };
}
