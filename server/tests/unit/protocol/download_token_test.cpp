#include <catch2/catch_test_macros.hpp>

#include "tds/protocol/download_token.hpp"
#include "tds/protocol/protocol_error.hpp"

#include <filesystem>
#include <fstream>

using namespace tds::protocol;
namespace fs = std::filesystem;

namespace {
    const fs::path test_directory = "/tmp/tds_protocol_DownloadToken_test";
    const fs::path file_name = test_directory / "file.txt";
    const fs::path dir_name = test_directory / "example";
    const std::string message = "Hello world!\n";
}

TEST_CASE("tds::protocol::DownloadToken", "[protocol]") {
    SECTION("Prepare files") {
        fs::create_directory(test_directory);
        std::ofstream{file_name} << message;
        fs::create_directory(dir_name);
    }

    SECTION("Test regular download token") {
        DownloadToken token{file_name};
        REQUIRE(token.get_file_path() == file_name);
        REQUIRE(token.get_file_size() == message.size());
        REQUIRE(token.get_file_offset() == 0);

        token.set_file_offset(2);
        REQUIRE(token.get_file_offset() == 2);
    }

    SECTION("Test invalid download token") {
        REQUIRE_THROWS_AS(DownloadToken{dir_name}, ProtocolError);
    }

    SECTION("Test 'make_download_token'") {
        auto token = make_download_token(file_name);
        REQUIRE(token->get_file_path() == file_name);
        REQUIRE(token->get_file_size() == message.size());
        REQUIRE(token->get_file_offset() == 0);

        token->set_file_offset(3);
        REQUIRE(token->get_file_offset() == 3);
    }

    SECTION("Remove files") {
        fs::remove_all(test_directory);
    }
}
