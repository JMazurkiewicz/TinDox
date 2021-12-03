#include <catch2/catch_test_macros.hpp>

#include "tds/ip/address_v4.hpp"
#include "tds/ip/ip_error.hpp"

#include <system_error>

using namespace tds::ip;
using namespace std::string_literals;

TEST_CASE("tds::ip::IpError", "[ip]") {
    SECTION("Test contructors (without error_category)") {
        IpError error1{1, "error"s};
        IpError error2{2, "error"};

        REQUIRE(error1.code() == std::error_code{1, std::system_category()});
        REQUIRE(error2.code() == std::error_code{2, std::system_category()});
    }

    SECTION("Test IpError thrown by `make_address_v4`") {
        try {
            [[maybe_unused]] auto address = make_address_v4("pi.wo");
        } catch(const IpError& e) {
            IpError error = e;
            REQUIRE(&error.code().category() == &get_gai_category());
        } catch(...) {
            REQUIRE(!"Invalid exception");
        }
    }
}
