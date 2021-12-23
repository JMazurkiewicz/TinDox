#include "tds/protocol/code.hpp"

namespace tds::protocol {
    namespace {
        class TdpCategory : public std::error_category {
        public:
            const char* name() const noexcept override {
                return "tdp";
            }

            std::string message(int condition) const override {
                using enum Code;
                switch(static_cast<Code>(condition)) {
                case ok:
                    return "no error";

                case line_too_long:
                    return "received line is too long";

                case not_enough_perms:
                    return "not enough permissions";

                case not_logged_in:
                    return "not logged in";

                case invalid_field:
                    return "invalid field in request";

                case not_found:
                    return "file not found";

                case target_not_found:
                    return "target directory not found";

                default:
                    return "unknown error";
                }
            }
        };

        inline const TdpCategory tdp_category;
    }

    const std::error_category& get_tdp_category() {
        return tdp_category;
    }
}
