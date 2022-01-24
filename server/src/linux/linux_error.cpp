#include "tds/linux/linux_error.hpp"

namespace tds::linux {
    LinuxError::LinuxError(const std::string& msg)
        : LinuxError(msg.c_str()) { }

    LinuxError::LinuxError(const char* msg)
        : LinuxError(errno, msg) { }

    LinuxError::LinuxError(int code, const std::string& msg)
        : LinuxError(code, msg.c_str()) { }

    LinuxError::LinuxError(int code, const char* msg)
        : system_error(code, std::system_category(), msg) { }
}
