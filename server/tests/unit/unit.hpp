#pragma once

#include <catch2/catch_test_macros.hpp>

#include <concepts>
#include <functional>

#include <sys/wait.h>
#include <unistd.h>

namespace tds::unit {
    // clang-format off
    template<typename T>
    concept TestableProcess = std::invocable<T> && std::same_as<std::invoke_result_t<T>, int>;
    // clang-format on

    template<TestableProcess Test>
    void test_in_new_process(Test&& test) {
        if(const int pid = fork(); pid == -1) {
            REQUIRE_FALSE("`test_in_new_process` failed to create process with fork(2)");
        } else if(pid == 0) {
            const int status = std::invoke(std::forward<Test>(test));
            _exit(status);
        } else {
            int status = 1;
            waitpid(pid, &status, 0);

            REQUIRE(WIFEXITED(status));
            REQUIRE(WEXITSTATUS(status) == 0);
        }
    }
}
