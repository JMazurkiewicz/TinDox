#include "tds/cli/run_command.hpp"

#include "tds/cli/invalid_command_arguments_error.hpp"
#include "tds/cli/invalid_command_execution_error.hpp"
#include "tds/cli/no_such_command_error.hpp"
#include "tds/config/config_reader.hpp"
#include "tds/server/server.hpp"
#include "tds/server/server_logger.hpp"

#include <charconv>
#include <filesystem>
#include <stdexcept>
#include <string_view>

#include <fmt/core.h>
#include <fmt/ostream.h>

namespace fs = std::filesystem;

namespace tds::cli {
    namespace {
        inline constexpr const char* port_argument_tip = "tds run --port <16 bit unsigned integer>";
        inline constexpr const char* config_directory_argument_tip = "tds run --dir <path to instance>";
    }

    void RunCommand::parse_arguments(std::span<const std::string_view> args) {
        const auto args_end = args.end();

        for(auto it = args.begin(); it != args_end; ++it) {
            if(*it == "--port" || *it == "-p") {
                if(++it == args_end) {
                    throw InvalidCommandArgumentsError{"--port (-p) expects port as argument", port_argument_tip};
                } else {
                    parse_port(*it);
                }
            } else if(*it == "--dir" || *it == "-d") {
                if(++it == args_end) {
                    throw InvalidCommandArgumentsError{"--dir (-d) expects valid path as argument",
                                                       config_directory_argument_tip};
                } else {
                    parse_config_directory_path(*it);
                }
            } else {
                throw InvalidCommandArgumentsError{std::string{*it}, "tds run"};
            }
        }
    }

    void RunCommand::execute() {
        prepare_config_directory();
        read_config();
        overwrite_port_if_possible();
        launch_server();
    }

    void RunCommand::parse_port(std::string_view arg) {
        std::uint16_t port = 0;
        const char* const arg_end = arg.data() + arg.size();
        auto&& [end, errc] = std::from_chars(arg.data(), arg_end, port);

        if(errc != std::errc{} || end != arg_end || port == 0) {
            throw InvalidCommandArgumentsError{"invalid --port value", port_argument_tip};
        } else {
            m_port.emplace(port);
        }
    }

    void RunCommand::parse_config_directory_path(std::string_view arg) {
        const fs::path instance_path = arg;
        const fs::path tds_config = instance_path / ".tds";
        if(!fs::exists(tds_config)) {
            throw InvalidCommandExecutionError{fmt::format("{} is not valid tds config directory", tds_config)};
        } else {
            m_config_directory.emplace(fs::canonical(instance_path));
        }
    }

    void RunCommand::prepare_config_directory() {
        if(!m_config_directory.has_value()) {
            parse_config_directory_path(fs::current_path().native());
        }
    }

    void RunCommand::read_config() {
        config::ConfigReader reader{*m_config_directory};
        m_config = reader.read_config();
    }

    void RunCommand::overwrite_port_if_possible() {
        if(m_port.has_value()) {
            m_config.set_port(ip::Port{*m_port});
        }
    }

    void RunCommand::launch_server() {
        server::Server server{*m_config_directory};
        server.set_config(m_config);

#ifndef NDEBUG
        server::server_logger->set_level(spdlog::level::debug);
#endif

        server.launch();
    }
}
