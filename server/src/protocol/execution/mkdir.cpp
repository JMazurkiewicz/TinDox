#include "tds/protocol/execution/mkdir.hpp"

#include "tds/protocol/protocol_error.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Mkdir::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "name") {
                parse_name(field);
            }
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name field is missing"};
        }
    }

    void Mkdir::execute() {
        const fs::path full_path = m_client_context->get_current_path() / *m_name;

        if(fs::exists(full_path)) {
            throw ProtocolError{ProtocolCode::file_already_exists, "this directory already exists"};
        } else {
            fs::create_directory(full_path);
        }
    }

    void Mkdir::parse_name(const Field& name_field) {
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
