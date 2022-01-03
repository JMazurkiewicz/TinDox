#include "tds/protocol/execution/exit.hpp"

namespace tds::protocol::execution {
    void Exit::parse_fields(std::span<const Field> fields) { }

    void Exit::execute() {
        m_client_context->kill();
    }
}
