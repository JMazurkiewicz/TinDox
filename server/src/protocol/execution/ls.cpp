#include "tds/protocol/execution/ls.hpp"

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Ls::parse_fields(std::span<const Field> fields) {
        /// TODO parse optional fields
    }

    void Ls::execute() {
        m_response_builder.set_code(ProtocolCode::ok);
        m_response_builder.set_command_name("ls");

        for(auto item : fs::directory_iterator{m_client_context->get_current_path()}) {
            const fs::path relative = m_server_context->get_relative_path_of(item.path());
            m_response_builder.add_line(relative.native());
        }
    }
}
