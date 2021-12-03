#include "tds/ip/ip_error.hpp"

#include <system_error>

namespace tds::ip {
    IpError::IpError(int code, const std::string& msg)
        : IpError(code, msg.c_str()) { }

    IpError::IpError(int code, const char* msg)
        : IpError(code, std::system_category(), msg) { }

    IpError::IpError(int code, const std::error_category& category, const std::string& msg)
        : IpError(code, category, msg.c_str()) { }

    IpError::IpError(int code, const std::error_category& category, const char* msg)
        : system_error(code, category, msg) { }
}
