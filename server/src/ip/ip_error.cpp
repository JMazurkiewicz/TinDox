#include "tds/ip/ip_error.hpp"

#include <system_error>

#include <netdb.h>

namespace tds::ip {
    IpError::IpError(int code, const std::string& msg)
        : IpError(code, msg.c_str()) { }

    IpError::IpError(int code, const char* msg)
        : IpError(code, std::system_category(), msg) { }

    IpError::IpError(int code, const std::error_category& category, const std::string& msg)
        : IpError(code, category, msg.c_str()) { }

    IpError::IpError(int code, const std::error_category& category, const char* msg)
        : system_error(code, category, msg) { }

    namespace {
        class GaiCategory : public std::error_category {
        public:
            const char* name() const noexcept override {
                return "gai";
            }

            std::string message(int condition) const override {
                return gai_strerror(condition);
            }
        };

        inline const GaiCategory gai_category;
    }

    const std::error_category& get_gai_category() {
        return gai_category;
    }
}
