#include <catch2/catch_test_macros.hpp>

#include "tds/config/defaults.hpp"

#include <thread>

using namespace tds::config;

TEST_CASE("tds::config::defaults::{}", "[config]") {
    const int hardware_concurrency = std::thread::hardware_concurrency();
    REQUIRE(defaults::get_default_max_thread_count() == hardware_concurrency);
    REQUIRE(defaults::get_default_max_user_count() == 5 * hardware_concurrency);
    REQUIRE(defaults::get_default_backlog() == 32);
}
