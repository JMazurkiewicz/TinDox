#include <catch2/catch_test_macros.hpp>

#include "tds/config/defaults.hpp"

#include <limits>
#include <thread>

using namespace tds::config;

TEST_CASE("tds::config::defaults::{}", "[config]") {
    const int hardware_concurrency = std::thread::hardware_concurrency();
    REQUIRE(defaults::get_default_max_thread_count() == hardware_concurrency);
    REQUIRE(defaults::get_default_max_user_count() == 5 * hardware_concurrency);
    REQUIRE(defaults::get_default_backlog() == 32);
    REQUIRE(defaults::get_default_port() == tds::ip::Port{37666});
    REQUIRE(defaults::get_default_upload_max_size() <= std::numeric_limits<int>::max());
}
