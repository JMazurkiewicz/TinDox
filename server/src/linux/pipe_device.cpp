#include "tds/linux/pipe_device.hpp"

#include <fcntl.h>
#include <unistd.h>

namespace tds::linux {
    PipeReadDevice::PipeReadDevice(int fd)
        : IoDevice(fd) { }

    PipeWriteDevice::PipeWriteDevice(int fd)
        : IoDevice(fd) { }

    PipeDevices make_pipe(bool blocking) {
        int fds[2];
        const int flags = (blocking ? 0 : O_NONBLOCK);
        pipe2(fds, flags);
        return {PipeReadDevice{fds[0]}, PipeWriteDevice{fds[1]}};
    }
}
