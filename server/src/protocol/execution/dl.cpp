#include "tds/protocol/execution/dl.hpp"

#include "tds/protocol/download_token.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <filesystem>

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Dl::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "name") {
                parse_name(field);
            }
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "dl requires name field"};
        }
    }

    void Dl::execute() {
        m_client_context->set_download_token(
            m_server_context->download_file(m_client_context->get_current_path() / *m_name));
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
}
