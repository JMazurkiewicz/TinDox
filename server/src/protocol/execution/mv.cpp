#include "tds/protocol/execution/mv.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Mv::execute() {
        // TODO block moving file if it is being downloaded by other users
        std::error_code errc;
        fs::rename(m_client_context->get_current_path() / get_name(), get_path() / get_name(), errc);

        if(const int code = errc.value(); code == EEXIST) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        } else if(code == EOPNOTSUPP) {
            throw ProtocolError{ProtocolCode::invalid_file_type, fmt::format("{} is not valid file", get_name())};
        } else if(code != 0) {
            throw ProtocolError{ProtocolCode::unknown,
                                fmt::format("unknown filesystem error: failed to move {} to {} ({})", get_name(),
                                            m_server_context->get_relative_path_of(get_path()), errc)};
        }
    }
}
