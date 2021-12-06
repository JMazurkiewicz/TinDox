#pragma once

#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <functional>

#include <sys/wait.h>
#include <unistd.h>

namespace tds::unit {
    template<std::invocable Test>
    void test_in_new_process(Test&& test) {
        switch(const int pid = fork()) {
        case -1:
            REQUIRE_FALSE("`test_in_new_process` failed to create process with fork(2)");

        case 0:
            std::invoke(std::forward<Test>(test));
            _exit(1);

        default: {
            int status = 1;
            waitpid(pid, &status, 0);

            REQUIRE(WIFEXITED(status));
            REQUIRE(WEXITSTATUS(status) == 0);
        }
        }
    }
}
