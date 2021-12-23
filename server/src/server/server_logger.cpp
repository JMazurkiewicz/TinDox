#include "tds/server/server_logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace tds::server {
    std::shared_ptr<spdlog::logger> server_logger = [] {
        auto logger = spdlog::stdout_color_mt("server_logger");
        logger->set_pattern("[%T %t] [%^%l%$] %v");
        return logger;
    }();
}
