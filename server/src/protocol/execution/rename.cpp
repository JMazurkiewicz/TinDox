#include "tds/protocol/execution/rename.hpp"

#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <filesystem>

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Rename::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "oname") {
                parse_name(field, m_oname);
            } else if(field.get_name() == "nname") {
                parse_name(field, m_nname);
            }
        }

        if(!m_oname.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "oname field is missing"};
        } else if(!m_nname.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "nname field is missing"};
        }
    }

    void Rename::execute() {
        const fs::path old_path = m_client_context->get_current_path() / *m_oname;
        if(m_server_context->is_path_locked(old_path)) {
            throw ProtocolError{ProtocolCode::in_use};
        }

        const fs::path new_path = m_client_context->get_current_path() / *m_nname;
        if(fs::exists(new_path)) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        }

        std::error_code errc;
        fs::rename(old_path, new_path, errc);

        if(const int code = errc.value(); code == EEXIST) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        } else if(code == EOPNOTSUPP) {
            throw ProtocolError{ProtocolCode::invalid_file_type, fmt::format("{} is not valid file", *m_oname)};
        } else if(code != 0) {
            throw ProtocolError{ProtocolCode::unknown,
                                fmt::format("unknown filesystem error: failed to rename {} file to {} ({})", *m_oname,
                                            *m_nname, errc)};
        }
    }

    void Rename::parse_name(const Field& name_field, std::optional<std::filesystem::path>& out) {
        const std::string_view field_name = name_field.get_name();
        if(out.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, fmt::format("{} was already set", field_name)};
        }

        const auto name_value = name_field.get_string();
        if(!name_value.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                fmt::format("{} field has invalid value type", field_name)};
        }

        fs::path name_as_path = *name_value;
        if(name_as_path != name_as_path.filename()) {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                fmt::format("{} must be just a file name", field_name)};
        }

        out.emplace(std::move(name_as_path));
    }
}
