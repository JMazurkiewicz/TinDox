#include <catch2/catch_test_macros.hpp>

#include "tds/config/defaults.hpp"
#include "tds/config/server_config.hpp"

#include <stdexcept>

using namespace tds::config;

TEST_CASE("tds::config::ServerConfig", "[config]") {
    SECTION("Test default constructor") {
        ServerConfig config;
        REQUIRE(config.get_max_thread_count() == defaults::get_default_max_thread_count());
        REQUIRE(config.get_max_user_count() == defaults::get_default_max_user_count());
        REQUIRE(config.get_backlog() == defaults::get_default_backlog());
        REQUIRE(config.get_port() == defaults::get_default_port());
    }

    SECTION("Test setters with good values") {
        ServerConfig config;

        config.set_max_thread_count(8);
        REQUIRE(config.get_max_thread_count() == 8);
        config.set_max_user_count(200);
        REQUIRE(config.get_max_user_count() == 200);
        config.set_backlog(64);
        REQUIRE(config.get_backlog() == 64);

        const tds::ip::Port port{65000};
        config.set_port(port);
        REQUIRE(config.get_port() == port);
    }

    SECTION("Test setters with bad values") {
        ServerConfig config;

        REQUIRE_THROWS_AS(config.set_max_thread_count(1), std::runtime_error);
        REQUIRE_THROWS_AS(config.set_max_user_count(0), std::runtime_error);
        REQUIRE_THROWS_AS(config.set_backlog(0), std::runtime_error);
        REQUIRE_THROWS_AS(config.set_port(tds::ip::Port::invalid), std::runtime_error);
    }
}
