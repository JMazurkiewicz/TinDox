#include "tds/protocol/execution/cp.hpp"

#include "tds/protocol/protocol_error.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Cp::execute() {
        std::error_code errc;
        const fs::path old_path = m_client_context->get_current_path() / get_name();
        fs::path new_path = get_path() / get_name();

        for(int copy_number = 1; fs::exists(new_path); ++copy_number) {
            const fs::path new_name = fmt::format("{} (copy {})", get_name().native(), copy_number);
            new_path = get_path() / new_name;
        }

        fs::copy_file(old_path, new_path, errc);

        if(const int code = errc.value(); code == EEXIST) {
            throw ProtocolError{ProtocolCode::file_already_exists};
        } else if(code == EOPNOTSUPP) {
            throw ProtocolError{ProtocolCode::invalid_file_type, fmt::format("{} is not valid file", get_name())};
        } else if(code != 0) {
            throw ProtocolError{ProtocolCode::unknown,
                                fmt::format("unknown filesystem error: failed to copy {} to {} ({})", get_name(),
                                            m_server_context->get_relative_path_to(get_path()), errc)};
        }
    }
}
