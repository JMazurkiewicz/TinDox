#include "tds/protocol/execution/tree.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Tree::parse_fields(std::span<const Field> fields) {
        PathBasedCommand::parse_fields(fields);
    }

    void Tree::execute() {
        if(!has_path()) {
            set_path(m_client_context->get_current_path());
        }

        for(fs::recursive_directory_iterator it{get_path()}; it != fs::recursive_directory_iterator{}; ++it) {
            std::string line(it.depth(), ' ');
            line += '"' + it->path().filename().native() + '"';

            if(fs::is_directory(it->path()) && fs::is_empty(it->path())) {
                line += " E";
            }

            m_response_builder.add_line(line);
        }
    }
}
