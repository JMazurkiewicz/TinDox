#include "tds/protocol/execution/dls.hpp"

#include "tds/protocol/protocol_mode.hpp"

#include <algorithm>

using namespace std::string_view_literals;

namespace tds::protocol::execution {
    void Dls::parse_fields(std::span<const Field> fields) { }

    void Dls::execute() {
        if(m_client_context->has_download_token()) {
            m_client_context->set_mode(ProtocolMode::download);
        } else {
            throw ProtocolError{ProtocolCode::dls_without_dl};
        }
    }

}
