#include "tds/server/server_logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace tds::server {
    namespace {
        auto make_server_logger() {
            auto logger = spdlog::stdout_color_mt("server_logger");
            logger->set_pattern("[%T.%f Thread:%t] [%^%l%$] %v");
            return logger;
        }
    }

    std::shared_ptr<spdlog::logger> server_logger = make_server_logger();
}
