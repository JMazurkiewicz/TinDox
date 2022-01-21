#include <catch2/catch_test_macros.hpp>

#include "tds/linux/file_device.hpp"
#include "tds/linux/pipe_device.hpp"
#include "tds/linux/transfer_bytes.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>

#include <fcntl.h>

using namespace tds::linux;
namespace fs = std::filesystem;

TEST_CASE("tds::linux::transfer_bytes", "[linux]") {
    constexpr const char* file_name = "/tmp/tds_transfer_bytes_test";
    constexpr std::string_view message = "sendfile64(2)";

    {
        std::ofstream ofile{file_name};
        ofile.write(message.data(), message.size());
    }

    auto [read_pipe, write_pipe] = make_pipe();

    FileDevice ifile;
    ifile.open(file_name, O_RDONLY);

    const std::size_t count = fs::file_size(file_name);
    REQUIRE(count == message.size());

    off64_t offset = 0;
    std::errc errc = {};
    const ssize_t transfered_bytes = transfer_bytes(ifile, write_pipe, offset, count, errc);

    REQUIRE(errc == std::errc{});
    REQUIRE(offset == count);
    REQUIRE(transfered_bytes == message.size());

    std::array<char, message.size()> buffer;
    const ssize_t read_bytes = read_pipe.read(buffer.data(), buffer.size());
    REQUIRE(read_bytes == message.size());
    REQUIRE(std::ranges::equal(buffer, message));
    REQUIRE_NOTHROW(fs::remove(file_name));
}
