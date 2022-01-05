#include "tds/protocol/execution/dls.hpp"

#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_mode.hpp"

#include <algorithm>

using namespace std::string_view_literals;

namespace tds::protocol::execution {
    Dls::Dls()
        : m_offset{0} { }

    void Dls::parse_fields(std::span<const Field> fields) {
        if(const auto it = std::ranges::find(fields, "offset"sv, &Field::get_name); it != fields.end()) {
            parse_offset(*it);
        }
    }

    void Dls::execute() {
        if(m_client_context->has_download_token()) {
            m_client_context->set_downloaded_file_offset(m_offset);
            m_client_context->set_mode(ProtocolMode::download);
        } else {
            throw ProtocolError{ProtocolCode::dls_without_dl};
        }
    }

    void Dls::parse_offset(const Field& offset_field) {
        const auto offset = offset_field.get_integer();
        if(!offset.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "offset has invalid value type (should be integer)"};
        } else {
            m_offset = *offset;
        }
    }
}
