#include "tds/protocol/execution/transport_command.hpp"

#include <algorithm>

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;
using namespace std::string_view_literals;

namespace tds::protocol::execution {
    void TransportCommand::parse_fields(std::span<const Field> fields) {
        PathBasedCommand::parse_fields(fields);
        if(const auto it = std::ranges::find(fields, "name"sv, &Field::get_name); it != fields.end()) {
            parse_name(*it);
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name field is missing"};
        }

        if(!has_path()) {
            throw ProtocolError{ProtocolCode::bad_field, "transport commands require path field"};
        }
    }

    const fs::path& TransportCommand::get_name() const {
        return m_name.value();
    }

    void TransportCommand::parse_name(const Field& name_field) {
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
        } else if(!fs::exists(m_client_context->get_current_path() / name_as_path)) {
            throw ProtocolError{ProtocolCode::not_found, fmt::format("file {} does not exist", name_as_path)};
        }

        m_name.emplace(std::move(name_as_path));
    }
}
