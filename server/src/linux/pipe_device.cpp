#include "tds/linux/pipe_device.hpp"

#include "tds/linux/linux_error.hpp"

#include <array>

#include <fcntl.h>
#include <unistd.h>

namespace tds::linux {
    PipeReadDevice::PipeReadDevice(int fd)
        : IoDevice(fd) { }

    PipeWriteDevice::PipeWriteDevice(int fd)
        : IoDevice(fd) { }

    PipeDevices make_pipe() {
        if(std::array<int, 2> fds; pipe2(fds.data(), 0) == -1) {
            throw LinuxError{"pipe2(2)"};
        } else {
            return {PipeReadDevice{fds[0]}, PipeWriteDevice{fds[1]}};
        }
    }
}
