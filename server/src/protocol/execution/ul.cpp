#include "tds/protocol/execution/ul.hpp"

#include "tds/protocol/protocol_error.hpp"

#include <algorithm>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Ul::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "name") {
                parse_name(field);
            } else if(field.get_name() == "size") {
                parse_size(field);
            } else if(field.get_name() == "retry") {
                parse_retry(field);
            }
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name field is missing"};
        } else if(!m_size.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "size field is missing"};
        }
    }

    void Ul::execute() { }

    void Ul::parse_name(const Field& name_field) {
        if(m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name was already set"};
        }

        const auto name_value = name_field.get_string();
        if(!name_value.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "name field has invalid value type (should be string)"};
        }

        fs::path name_as_path = *name_value;
        if(name_as_path != name_as_path.filename()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "name must be just a name"};
        } else {
            m_name.emplace(std::move(name_as_path));
        }
    }

    void Ul::parse_size(const Field& size_field) {
        const auto size = size_field.get_integer();
        if(!size.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "size has invalid value type (should be integer)"};
        } else {
            m_size = *size;
        }
    }

    void Ul::parse_retry(const Field& retry_field) {
        if(auto retry_value = retry_field.get_boolean()) {
            m_retry = *retry_value;
        } else {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "retry field has invalid value type (should be boolean)"};
        }
    }
}
