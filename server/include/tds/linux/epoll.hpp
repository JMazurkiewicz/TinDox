#pragma once

#include "tds/linux/io_device.hpp"

#include <chrono>
#include <span>
#include <unordered_map>

#include <sys/epoll.h>

namespace tds::linux {
    class Epoll : public IoDevice {
    public:
        static constexpr std::chrono::milliseconds default_timeout{5000};

        Epoll();

        void add_device(IoDevice& dev);
        void handle() override;

    private:
        int get_event_count(std::span<epoll_event> events);
        IoDevice& get_device(epoll_event event);

        std::unordered_map<int, IoDevice*> m_devices;
    };
}
