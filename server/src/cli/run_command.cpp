#include "tds/cli/run_command.hpp"

#include "tds/config/config_reader.hpp"
#include "tds/linux/linux_error.hpp"
#include "tds/server/server.hpp"

#include <charconv>
#include <filesystem>
#include <stdexcept>

#include <fmt/core.h>

namespace fs = std::filesystem;

namespace tds::cli {
    void RunCommand::parse_arguments(std::span<const std::string_view> args) {
        const auto args_end = args.end();

        for(auto it = args.begin(); it != args_end; ++it) {
            if(*it == "--port") {
                if(++it == args_end) {
                    throw std::runtime_error{"--port expects port as argument"};
                } else {
                    parse_port(*it);
                }
            } else if(*it == "--path") {
                if(++it == args_end) {
                    throw std::runtime_error{"--port expects valid path as argument"};
                } else {
                    parse_root(*it);
                }
            } else {
                throw std::runtime_error{fmt::format("Invalid option {}", *it)};
            }
        }

        make_root_path();
        read_config();
        apply_arguments();
    }

    void RunCommand::execute() {
        server::Server server{*m_root};
        server.set_config(m_config);
        server.launch();
    }

    void RunCommand::parse_port(std::string_view arg) {
        std::uint16_t port = 0;
        const char* const arg_end = arg.data() + arg.size();
        auto&& [end, errc] = std::from_chars(arg.data(), arg_end, port);

        if(errc != std::errc{} || end != arg_end || port == 0) {
            throw std::runtime_error{
                fmt::format("invalid --port value (should be 16 bit unsigned integer, got {})", arg)};
        } else {
            m_port.emplace(port);
        }
    }

    void RunCommand::parse_root(std::string_view arg) {
        fs::path root = arg;
        if(!fs::exists(root)) {
            throw std::runtime_error{fmt::format("Path {} does not exist", root.native())};
        }

        const fs::path tds_config = fs::canonical(root) / ".tds";
        if(!fs::exists(tds_config)) {
            throw std::runtime_error{fmt::format("{} is not valid TDS instance", tds_config.native())};
        } else {
            m_root.emplace(std::move(root));
        }
    }

    void RunCommand::make_root_path() {
        if(!m_root.has_value()) {
            parse_root(fs::current_path().native());
        }
    }

    void RunCommand::read_config() {
        config::ConfigReader reader{*m_root};
        m_config = reader.read_config();
    }

    void RunCommand::apply_arguments() {
        if(m_port.has_value()) {
            m_config.set_port(ip::Port{*m_port});
        }
    }
}
