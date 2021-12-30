#pragma once

#include <cstdint>
#include <system_error>

namespace tds::protocol {
    enum class ProtocolCode : std::uint16_t {
        ok = 100,

        too_long_line = 300,
        too_many_fields = 301,
        bad_field = 302,

        not_logged_in = 402,
        invalid_field = 403,
        not_found = 404,
        target_not_found = 405,
        not_enough_perms = 406,
    };

    constexpr bool is_info_code(ProtocolCode code) noexcept {
        return static_cast<int>(code) / 100 == 1;
    }

    constexpr bool is_frame_error_code(ProtocolCode code) noexcept {
        return static_cast<int>(code) / 300 == 3;
    }

    constexpr bool is_error_code(ProtocolCode code) noexcept {
        return static_cast<int>(code) / 100 == 4;
    }
}

namespace std {
    template<>
    struct is_error_code_enum<tds::protocol::ProtocolCode> : true_type { };
}
