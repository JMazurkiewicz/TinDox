#pragma once

#include "tds/linux/io_device.hpp"

#include <functional>
#include <unordered_map>

#include <sys/signalfd.h>

namespace tds::linux {
    class SignalDevice : public IoDevice {
    public:
        using SignalHandler = std::function<void(int)>;

        SignalDevice() = default;

        void add_handler(int signal, SignalHandler handler);
        void start_signal_device();
        void handle_signal();

    protected:
        using IoDevice::read;
        using IoDevice::write;

    private:
        void create_mask();
        void block_signals();
        void create_fd();

        sigset_t m_mask;
        std::unordered_map<int, SignalHandler> m_handlers;
    };
}
