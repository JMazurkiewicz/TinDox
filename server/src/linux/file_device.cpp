#include "tds/linux/file_device.hpp"

#include "tds/linux/linux_error.hpp"

#include <fcntl.h>

namespace tds::linux {
    void FileDevice::open(const std::string& name, int flags) {
        open(name.c_str(), flags);
    }

    void FileDevice::open(const char* name, int flags) {
        const int fd = ::open(name, flags);
        if(fd == -1) {
            throw LinuxError{"open(2)"};
        } else {
            set_fd(fd);
        }
    }
}
