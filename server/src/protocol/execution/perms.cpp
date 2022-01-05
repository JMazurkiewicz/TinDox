#include "tds/protocol/execution/perms.hpp"

namespace tds::protocol::execution {
    void Perms::parse_fields(std::span<const Field> fields) { }

    void Perms::execute() {
        m_response_builder.add_line(user::to_string(m_client_context->get_auth_token().get_perms()));
    }
}
