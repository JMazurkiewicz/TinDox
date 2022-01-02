#include "tds/protocol/response_builder.hpp"

#include <fmt/core.h>

namespace tds::protocol {
    void ResponseBuilder::set_code(ProtocolCode code) {
        m_code = code;
    }

    void ResponseBuilder::set_command_name(std::string_view command_name) {
        m_command_name = command_name;
    }

    void ResponseBuilder::add_line(std::string_view line) {
        m_lines += line;
        m_lines += '\n';
    }

    Response ResponseBuilder::get_response() {
        return Response{fmt::format("{} {}\n{}\n", static_cast<int>(m_code), m_command_name, m_lines)};
    }
}
