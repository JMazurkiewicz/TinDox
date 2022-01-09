#include "tds/protocol/protocol_error.hpp"

#include "tds/protocol/protocol_code.hpp"

namespace tds::protocol {
    namespace {
        class TdpCategory : public std::error_category {
        public:
            const char* name() const noexcept override {
                return "tdp";
            }

            std::string message(int condition) const override {
                using enum ProtocolCode;
                switch(static_cast<ProtocolCode>(condition)) {
                case ok:
                    return "no error";

                case too_long_line:
                    return "received line is too long";

                case too_many_fields:
                    return "received too many fields";

                case bad_field:
                    return "invalid field";

                case bad_command:
                    return "invalid command";

                case not_enough_perms:
                    return "not enough permissions";

                case unknown:
                    return "unknown";

                case not_logged_in:
                    return "not logged in";

                case invalid_field_value:
                    return "invalid field value in request";

                case not_found:
                    return "file not found";

                case no_upload_to_resume:
                    return "no upload to resume";

                case user_already_logged:
                    return "user is already logged in";

                case invalid_credentials:
                    return "invalid credentials (login or password)";

                case file_already_exists:
                    return "file or directory already exists";

                case invalid_file_type:
                    return "invalid file type";

                case dls_without_dl:
                    return "download start requested without dl command";

                case in_use:
                    return "file or directory is used by another user";

                case wrong_upload_path:
                    return "wrong upload path";

                case not_a_directory:
                    return "not a directory";

                case too_large_file:
                    return "too large file";
                    
                default:
                    return "unknown code";
                }
            }
        };

        inline const TdpCategory tdp_category;
    }

    const std::error_category& get_tdp_category() {
        return tdp_category;
    }

    ProtocolError::ProtocolError(ProtocolCode code)
        : ProtocolError(code, get_tdp_category().message(static_cast<int>(code))) { }

    ProtocolError::ProtocolError(ProtocolCode code, const std::string& message)
        : ProtocolError(code, message.c_str()) { }

    ProtocolError::ProtocolError(ProtocolCode code, const char* message)
        : runtime_error(message)
        , m_code{code} { }

    ProtocolCode ProtocolError::get_code() const noexcept {
        return m_code;
    }
}
