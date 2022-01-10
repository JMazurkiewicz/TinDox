#include <catch2/catch_test_macros.hpp>

#include "tds/linux/file_device.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <system_error>

#include <fcntl.h>

using namespace tds::linux;
namespace fs = std::filesystem;

inline constexpr const char* file_name = "/tmp/tds_file_device_test";
inline constexpr std::string_view message = "FileDevice";
inline const std::string file_name_str = file_name;

TEST_CASE("tds::linux::FileDevice", "[linux]") {
    SECTION("Create and write to file") {
        FileDevice file;
        REQUIRE(file.get_fd() == -1);
        REQUIRE(!file.is_valid());
        REQUIRE(!file);

        REQUIRE_NOTHROW(file.open(file_name, O_WRONLY | O_APPEND | O_CREAT));
        REQUIRE(file.get_fd() > 2);
        REQUIRE(file.is_valid());
        REQUIRE(file);

        const ssize_t written_bytes = file.write(message.data(), message.size());
        REQUIRE(written_bytes == message.size());

        REQUIRE_NOTHROW(file.close());
        REQUIRE(fs::file_size(file_name) == message.size());
    }

    SECTION("Read from file and remove it") {
        FileDevice file;
        REQUIRE_NOTHROW(file.open(file_name_str, O_RDWR));
        REQUIRE(file.get_fd() > 2);
        REQUIRE(file.is_valid());
        REQUIRE(file);

        std::array<char, message.size()> buffer;
        const ssize_t read_bytes = file.read(buffer.data(), buffer.size());
        REQUIRE(read_bytes == message.size());
        REQUIRE(std::ranges::equal(buffer, message));

        REQUIRE_NOTHROW(file.close());
        REQUIRE_NOTHROW(fs::remove(file_name_str));
    }
}
