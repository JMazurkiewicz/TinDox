#pragma once

#include "tds/linux/io_device.hpp"

#include <functional>
#include <unordered_map>

#include <sys/signalfd.h>

namespace tds::linux {
    using SignalHandler = std::function<void(int)>;

    class SignalDevice : public IoDevice {
    public:
        SignalDevice() = default;

        void add_handler(int signal, SignalHandler handler);
        void apply();
        void handle_signal();

    private:
        void create_mask();
        void block_signals();
        void create_fd();

        sigset_t m_mask;
        std::unordered_map<int, SignalHandler> m_handlers;
    };
}
