#pragma once

#include "tds/linux/io_device.hpp"

namespace tds::linux {
    class PipeReadDevice : public IoDevice {
    public:
        explicit PipeReadDevice(int fd);

        PipeReadDevice(PipeReadDevice&&) = default;
        PipeReadDevice& operator=(PipeReadDevice&&) = default;

    private:
        using IoDevice::write;
    };

    class PipeWriteDevice : public IoDevice {
    public:
        explicit PipeWriteDevice(int fd);

        PipeWriteDevice(PipeWriteDevice&&) = default;
        PipeWriteDevice& operator=(PipeWriteDevice&&) = default;

    private:
        using IoDevice::read;
    };

    struct PipeDevices {
        PipeReadDevice m_read_device;
        PipeWriteDevice m_write_device;
    };

    PipeDevices make_pipe();
}
