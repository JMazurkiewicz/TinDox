#pragma once

#include "tds/linux/epoll_event.hpp"

#include <cstddef>
#include <memory>
#include <ranges>
#include <span>

#include <sys/epoll.h>

namespace tds::linux {
    class EpollBuffer {
    public:
        explicit EpollBuffer(std::size_t max_size);

        std::size_t size() const noexcept;
        void set_size(std::size_t size) noexcept;
        std::size_t max_size() const noexcept;

        epoll_event* data() noexcept;
        const epoll_event* data() const noexcept;

        auto get_events() const noexcept;

    private:
        std::size_t m_size;
        std::unique_ptr<epoll_event[]> m_buffer;
        std::size_t m_max_size;
    };

    inline auto EpollBuffer::get_events() const noexcept {
        std::span<epoll_event> available_events{m_buffer.get(), m_size};
        return available_events | std::views::transform([](const epoll_event& event) -> EpollEvent {
                   return {event.data.fd, static_cast<EventType>(event.events)};
               });
    }
}
