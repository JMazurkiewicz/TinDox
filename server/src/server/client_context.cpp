#include "tds/server/client_context.hpp"

namespace tds::server {
    ClientContext::ClientContext()
        : m_current_path{"/"} { }
}
