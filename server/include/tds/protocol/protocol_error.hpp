#pragma once

#include <stdexcept>
#include <system_error>

namespace tds::protocol {
    enum class ProtocolCode : std::uint16_t {
        ok = 100,

        too_long_line = 300,
        too_many_fields = 301,
        bad_field = 302,
        bad_command = 303,

        unknown = 401,
        not_logged_in = 402,
        invalid_field_value = 403,
        not_found = 404,
        no_upload_to_resume = 405,
        not_enough_perms = 406,
        user_already_logged = 407,
        invalid_credentials = 408,
        file_already_exists = 409,
        invalid_file_type = 410,
        dls_without_dl = 411,
        in_use = 412,
        wrong_upload_path = 413,
        not_a_directory = 414,
        too_large_file = 415,
    };
}

namespace std {
    template<>
    struct is_error_code_enum<tds::protocol::ProtocolCode> : true_type { };
}

namespace tds::protocol {
    std::error_code make_error_code(ProtocolCode code);
    const std::error_category& get_tdp_category();

    class ProtocolError : public std::runtime_error {
    public:
        explicit ProtocolError(ProtocolCode code);
        explicit ProtocolError(ProtocolCode code, const std::string& message);
        explicit ProtocolError(ProtocolCode code, const char* message);

        ProtocolCode get_code() const noexcept;

    private:
        ProtocolCode m_code;
    };
}
