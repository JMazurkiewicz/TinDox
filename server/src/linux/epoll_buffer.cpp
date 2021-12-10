#include "tds/linux/epoll_buffer.hpp"

namespace tds::linux {
    EpollBuffer::EpollBuffer(std::size_t max_size)
        : m_size{0}
        , m_buffer{std::make_unique<epoll_event[]>(max_size)}
        , m_max_size{max_size} { }

    std::size_t EpollBuffer::size() const noexcept {
        return m_size;
    }

    void EpollBuffer::set_size(std::size_t size) noexcept {
        m_size = size;
    }

    std::size_t EpollBuffer::max_size() const noexcept {
        return m_max_size;
    }

    epoll_event* EpollBuffer::data() noexcept {
        return m_buffer.get();
    }

    const epoll_event* EpollBuffer::data() const noexcept {
        return m_buffer.get();
    }
}
