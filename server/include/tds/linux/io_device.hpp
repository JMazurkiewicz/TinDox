#pragma once

namespace tds::linux {
    class IoDevice {
    public:
        IoDevice();
        explicit IoDevice(int fd);

        IoDevice(const IoDevice&) = delete;
        IoDevice& operator=(const IoDevice&) = delete;

        virtual ~IoDevice();

        bool is_valid() const noexcept;
        int get_fd() const noexcept;

        virtual void handle() = 0;

    protected:
        void set_fd(int fd);
        void raw_close();

    private:
        int m_fd;
    };
}
