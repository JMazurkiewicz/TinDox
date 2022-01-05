#include "tds/protocol/execution/mv.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Mv::execute() {
        const fs::path old_path = m_client_context->get_current_path() / get_name();
        if(m_server_context->is_locked(old_path)) {
            throw ProtocolError{ProtocolCode::in_use};
        }

        const fs::path new_path = get_path() / get_name();
        if(fs::exists(new_path)) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        }

        std::error_code errc;
        fs::rename(old_path, new_path, errc);
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
