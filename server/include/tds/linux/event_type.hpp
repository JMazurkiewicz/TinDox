#pragma once

#include <cstdint>
#include <type_traits>

#include <sys/epoll.h>

namespace tds::linux {
    enum class EventType : std::uint32_t {
        in = EPOLLIN,
        out = EPOLLOUT,
        edge_triggered = EPOLLET,
        one_shot = EPOLLONESHOT
    };

    constexpr EventType operator~(EventType type) {
        using I = std::underlying_type_t<EventType>;
        return static_cast<EventType>(~static_cast<I>(type));
    }

    constexpr EventType operator|(EventType lhs, EventType rhs) {
        using I = std::underlying_type_t<EventType>;
        return static_cast<EventType>(static_cast<I>(lhs) | static_cast<I>(rhs));
    }

    constexpr EventType operator&(EventType lhs, EventType rhs) {
        using I = std::underlying_type_t<EventType>;
        return static_cast<EventType>(static_cast<I>(lhs) & static_cast<I>(rhs));
    }

    constexpr EventType operator^(EventType lhs, EventType rhs) {
        using I = std::underlying_type_t<EventType>;
        return static_cast<EventType>(static_cast<I>(lhs) ^ static_cast<I>(rhs));
    }

    constexpr EventType operator|=(EventType& lhs, EventType rhs) {
        return lhs = lhs | rhs;
    }

    constexpr EventType operator&=(EventType& lhs, EventType rhs) {
        return lhs = lhs & rhs;
    }

    constexpr EventType operator^=(EventType& lhs, EventType rhs) {
        return lhs = lhs ^ rhs;
    }
}
