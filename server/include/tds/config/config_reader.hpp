#pragma once

#include "tds/config/server_config.hpp"

#include <filesystem>

#include <toml++/toml.h>

namespace tds::config {
    class ConfigReader {
    public:
        explicit ConfigReader(std::filesystem::path root_directory);

        ConfigReader(const ConfigReader&) = delete;
        ConfigReader& operator=(const ConfigReader&) = delete;

        ServerConfig read_config();

    private:
        void set_integer_field(std::string_view name, std::int64_t value);

        toml::table m_table;
        ServerConfig m_config;
    };
}
