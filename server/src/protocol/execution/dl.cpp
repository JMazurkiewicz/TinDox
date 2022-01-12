#include "tds/protocol/execution/dl.hpp"

#include "tds/protocol/download_token.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <filesystem>

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    Dl::Dl()
        : m_offset{0} { }

    void Dl::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "name") {
                parse_name(field);
            } else if(field.get_name() == "offset") {
                parse_offset(field);
            }
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "dl requires name field"};
        }
    }

    void Dl::execute() {
        auto token = m_server_context->download_file(m_client_context->get_current_path() / *m_name);
        token->set_file_offset(m_offset);
        m_response_builder.add_line(std::to_string(token->get_file_size() - token->get_file_offset()));
        m_client_context->set_download_token(std::move(token));
    }

    void Dl::parse_name(const Field& name_field) {
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

    void Dl::parse_offset(const Field& offset_field) {
        const auto offset = offset_field.get_integer();
        if(!offset.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "offset has invalid value type (should be integer)"};
        } else {
            m_offset = *offset;
        }
    }
}
