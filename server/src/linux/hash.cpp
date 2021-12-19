#include "tds/linux/hash.hpp"

#include "tds/linux/linux_error.hpp"

#include <array>
#include <cstring>

#include <unistd.h>

namespace tds::linux {
    namespace {
        inline constexpr std::string_view salt_chars =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

        const char* hash_impl(const char* str, std::size_t len) {
            auto salt = [&]() -> std::array<char, 3> {
                if(len >= 2) {
                    const unsigned char first = str[0];
                    const unsigned char last = str[len - 1];
                    return {salt_chars[(first ^ last) % salt_chars.size()],
                            salt_chars[(first * last) % salt_chars.size()], '\0'};
                } else {
                    return {'.', '0', '\0'};
                }
            }();

            if(const auto result = crypt(str, salt.data()); result == nullptr) {
                throw LinuxError{"linux::hash"};
            } else {
                return result;
            }
        }
    }

    std::string hash(const std::string& word) {
        return hash_impl(word.c_str(), word.size());
    }

    std::string hash(const char* word) {
        return hash_impl(word, std::strlen(word));
    }
}
