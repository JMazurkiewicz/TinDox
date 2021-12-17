#pragma once

#include <string>

namespace tds::linux {
    std::string hash(const std::string& word);
    std::string hash(const char* word);
}
