#include <catch2/catch_test_macros.hpp>

#include "tds/linux/signal_device.hpp"

#include <csignal>

#include <unistd.h>

using namespace tds::linux;

TEST_CASE("tds::linux::SignalDevice", "[linux]") {
    SignalDevice signal_device;
    bool flag = false;

    signal_device.add_handler(SIGINT, [&](int) { flag = true; });
    signal_device.apply();

    std::raise(SIGINT);
    signal_device.handle();

    REQUIRE(flag);
}
