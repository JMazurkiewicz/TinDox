#include "tds/linux/hash.hpp"

#include "tds/linux/linux_error.hpp"

#include <array>
#include <cstring>

#include <unistd.h>

namespace tds::linux {
    namespace {
        constexpr std::string_view salt_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

        const char* do_hash(const char* str, std::size_t len) {
            std::array<char, 3> salt;
            if(len >= 2) {
                const unsigned char first = str[0];
                const unsigned char last = str[len - 1];
                salt[0] = salt_chars[(first ^ last) % salt_chars.size()];
                salt[1] = salt_chars[(first * last) % salt_chars.size()];
            } else {
                salt[0] = '.';
                salt[1] = '0';
            }

            if(const auto result = crypt(str, salt.data()); result == nullptr) {
                throw LinuxError{"linux::hash"};
            } else {
                return result;
            }
        }
    }

    std::string hash(const std::string& word) {
        return do_hash(word.c_str(), word.size());
    }

    std::string hash(const char* word) {
        return do_hash(word, std::strlen(word));
    }
}
