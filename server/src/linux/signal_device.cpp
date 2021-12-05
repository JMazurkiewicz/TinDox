#include "tds/linux/signal_device.hpp"

#include "tds/linux/linux_error.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>

#include <signal.h>
#include <unistd.h>

namespace tds::linux {
    void SignalDevice::add_handler(int signal, SignalHandler handler) {
        m_handlers[signal] = std::move(handler);
    }

    void SignalDevice::apply() {
        create_mask();
        block_signals();
        create_fd();
    }

    void SignalDevice::handle() {
        signalfd_siginfo info;
        if(sizeof(info) != read(get_fd(), &info, sizeof(info))) {
            throw LinuxError{"read(2)"};
        }

        const auto signo = info.ssi_signo;
        auto handler = m_handlers.find(signo);

        if(handler != m_handlers.end()) {
            handler->second(signo);
        } else {
            throw std::runtime_error{"No handler for signal with '" + std::to_string(signo) + "' code"};
        }
    }

    void SignalDevice::create_mask() {
        sigemptyset(&m_mask);
        std::ranges::for_each(m_handlers | std::views::keys, std::bind_front(sigaddset, &m_mask));
    }

    void SignalDevice::block_signals() {
        if(sigprocmask(SIG_BLOCK, &m_mask, nullptr) == -1) {
            throw LinuxError{"sigprocmask(2)"};
        }
    }

    void SignalDevice::create_fd() {
        const int fd = signalfd(-1, &m_mask, 0);
        if(fd == -1) {
            throw LinuxError{"signalfd(2)"};
        } else {
            set_fd(fd);
        }
    }
}
