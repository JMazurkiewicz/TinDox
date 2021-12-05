#pragma once

#include <system_error>

namespace tds::linux {
    class LinuxError : public std::system_error {
    public:
        explicit LinuxError(const std::string& msg);
        explicit LinuxError(const char* msg);

        explicit LinuxError(int code, const std::string& msg);
        explicit LinuxError(int code, const char* msg);
    };
}
