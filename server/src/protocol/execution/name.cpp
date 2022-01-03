#include "tds/protocol/execution/name.hpp"

namespace tds::protocol::execution {
    void Name::parse_fields(std::span<const Field> fields) { }

    void Name::execute() {
        m_response_builder.add_line(m_client_context->get_auth_token().get_username());
    }
}
