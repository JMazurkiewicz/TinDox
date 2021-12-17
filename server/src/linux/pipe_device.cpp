#include "tds/linux/pipe_device.hpp"

#include "tds/linux/linux_error.hpp"

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

        if(pipe2(fds, flags) == -1) {
            throw LinuxError{"pipe2(2)"};
        } else {
            return {PipeReadDevice{fds[0]}, PipeWriteDevice{fds[1]}};
        }
    }
}
