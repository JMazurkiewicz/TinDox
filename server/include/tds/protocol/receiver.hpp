#pragma once

#include "tds/linux/io_device.hpp"

#include <span>
#include <vector>

namespace tds::protocol {
    class Receiver {
    public:
        Receiver();
        Receiver(const Receiver&) = delete;
        Receiver& operator=(const Receiver&) = delete;

        void set_device(linux::IoDevice& device);
        std::span<const char> receive();

    private:
        linux::IoDevice* m_dev;
        std::vector<char> m_buffer;
    };
}
