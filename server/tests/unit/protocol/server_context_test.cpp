#include <catch2/catch_test_macros.hpp>

#include "tds/cli/init_command.hpp"
#include "tds/config/server_config.hpp"
#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/server_context.hpp"

#include <filesystem>

using namespace tds::protocol;
namespace fs = std::filesystem;
using namespace std::string_view_literals;

TEST_CASE("tds::protocol::ServerContext", "[protocol]") {
    const fs::path root = "/tmp/tds_server_context_test";

    SECTION("Create tds instance") {
        fs::create_directory(root);
        fs::create_directory(root / "outer");
        fs::create_directory(root / "outer/inner");

        tds::cli::InitCommand init;
        init.parse_arguments(std::array{std::string_view{root.native()}});
        init.execute();
    }

    tds::config::ServerConfig config;
    ServerContext context{root, config};

    SECTION("Test constructor") {
        REQUIRE(context.get_root_path() == root);
    }

    SECTION("Test 'get_relative_path_to") {
        REQUIRE(context.get_relative_path_to(root / "test") == "/test");
        REQUIRE(context.get_relative_path_to(root) == "/");
    }

    SECTION("Test 'authorize_user'") {
        const std::string name = "admin";

        auto auth_token = context.authorize_user(name, name);
        REQUIRE(auth_token != nullptr);
        REQUIRE_THROWS_AS(context.authorize_user(name, name), ProtocolError);

        auth_token.reset();
        REQUIRE(context.authorize_user(name, name) != nullptr);
    }

    SECTION("Test 'lock_path' and 'is_path_locked'") {
        const fs::path inner = root / "outer/inner";

        auto lock = context.lock_path(inner);
        REQUIRE(context.is_path_locked(inner));
        REQUIRE(context.is_path_locked(root / "outer"));
        REQUIRE(context.is_path_locked(root));

        lock.reset();
        REQUIRE(!context.is_path_locked(inner));
        REQUIRE(!context.is_path_locked(root / "outer"));
        REQUIRE(!context.is_path_locked(root));
    }

    SECTION("Test 'is_path_forbidden'") {
        REQUIRE(context.is_path_forbidden(root / ".tds"));
        REQUIRE(!context.is_path_forbidden(root / "tds"));
    }

    SECTION("Remove tds instance") {
        fs::remove_all(root);
    }
}
