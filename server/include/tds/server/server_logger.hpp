#pragma once

#include <fmt/ostream.h>
#include <spdlog/logger.h>

namespace tds::server {
    extern std::shared_ptr<spdlog::logger> server_logger;
}
