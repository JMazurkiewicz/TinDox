#pragma once

#include "tds/server/server_context.hpp"

#include <optional>
#include <string>

namespace tds::server {
    class UserContext {
    public:
        UserContext();

        UserContext(const UserContext&) = delete;
        UserContext& operator=(const UserContext&) = delete;
    };
}
