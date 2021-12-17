#include "tds/linux/hash.hpp"

#include "tds/linux/linux_error.hpp"

#include <unistd.h>

namespace tds::linux {
    std::string hash(const std::string& word) {
        return hash(word.c_str());
    }

    std::string hash(const char* word) {
        if(const auto result = crypt(word, "XD"); result == nullptr) {
            throw LinuxError{"hash"};
        } else {
            return result;
        }
    }
}
