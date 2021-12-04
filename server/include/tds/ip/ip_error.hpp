#pragma once

#include <system_error>

namespace tds::ip {
    class IpError : public std::system_error {
    public:
        explicit IpError(int code, const std::string& msg);
        explicit IpError(int code, const char* msg);

        explicit IpError(int code, const std::error_category& category, const std::string& msg);
        explicit IpError(int code, const std::error_category& category, const char* msg);
    };

    const std::error_category& get_gai_category();
}
