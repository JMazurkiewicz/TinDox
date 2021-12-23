#pragma once

#include <system_error>

namespace tds::protocol {
    enum class Code : std::uint16_t {
        ok = 100,

        line_too_long = 400,
        not_enough_perms = 401,
        not_logged_in = 402,
        invalid_field = 403,
        not_found = 404,
        target_not_found = 405,
    };
}

namespace std {
    template<>
    struct is_error_code_enum<tds::protocol::Code> : true_type { };
}

namespace tds::protocol {
    const std::error_category& get_tdp_category();
}
