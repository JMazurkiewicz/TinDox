#pragma once

#include "tds/linux/io_device.hpp"

namespace tds::linux {
    class FileDevice : public IoDevice {
    public:
        FileDevice() = default;

        void open(const std::string& name, int flags);
        void open(const char* name, int flags);
    };
}
