#include "tds/config/config_reader.hpp"

#include "tds/config/server_config.hpp"

#include <stdexcept>

#include <fmt/core.h>

namespace tds::config {
    ConfigReader::ConfigReader(std::filesystem::path root_directory)
        : m_table{toml::parse_file((root_directory / ".tds/config").native())} { }

    ServerConfig ConfigReader::read_config() {
        const auto server_config = m_table["server"];
        if(!server_config) {
            throw std::runtime_error{"Section [server] does not exist in config file!"};
        }

        if(auto table = server_config.as_table()) {
            for(auto&& [name, value] : *table) {
                if(auto integer = value.as_integer()) {
                    set_integer_field(name, integer->get());
                } else {
                    throw std::runtime_error{
                        fmt::format("ConfigReader: field '{}' is invalid", name)};
                }
            }
        }

        return m_config;
    }

    void ConfigReader::set_integer_field(std::string_view name, std::int64_t value) {
        if(name == "max_thread_count") {
            m_config.set_max_thread_count(value);
        } else if(name == "max_user_count") {
            m_config.set_max_user_count(value);
        } else if(name == "backlog") {
            m_config.set_backlog(value);
        } else if(name == "port") {
            if(!std::in_range<std::uint16_t>(value)) {
                throw std::runtime_error{fmt::format("ConfigReader: invalid port value in config file ({})", value)};
            } else {
                m_config.set_port(ip::Port(value));
            }
        } else if(name == "upload_max_size") {
            m_config.set_upload_max_size(value);
        } else {
            throw std::runtime_error{fmt::format("ConfigReader: {} is not valid integer field", name)};
        }
    }
}
