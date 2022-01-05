#include "tds/protocol/execution/rm.hpp"

#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <filesystem>

#include <fmt/core.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Rm::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "name") {
                parse_name(field);
            }
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name field is missing"};
        }
    }

    void Rm::execute() {
        const fs::path full_path = m_client_context->get_current_path() / *m_name;
        if(!fs::exists(full_path)) {
            throw ProtocolError{ProtocolCode::not_found};
        } else if(m_server_context->is_locked(full_path)) {
            throw ProtocolError{ProtocolCode::in_use};
        } else {
            const std::uintmax_t count = fs::remove_all(full_path);
            m_response_builder.add_line(fmt::format("Removed {} entries", count));
        }
    }

    void Rm::parse_name(const Field& name_field) {
        if(m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name was already set"};
        }

        const auto name_value = name_field.get_string();
        if(!name_value.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "name field has invalid value type"};
        }

        fs::path name_as_path = *name_value;
        if(name_as_path != name_as_path.filename()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "name must be just a name"};
        }

        m_name.emplace(std::move(name_as_path));
    }
}
