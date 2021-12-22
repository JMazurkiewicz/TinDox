#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

namespace tds::user {
    enum class Permissions : std::uint8_t {
        travel = 1 << 0,
        write = 1 << 1,
        copy = 1 << 2,
        move = 1 << 3,
        download = 1 << 4,
        upload = 1 << 5,

        initial = travel | download,
        all = travel | write | copy | move | download | upload,
    };

    constexpr Permissions operator~(Permissions perms) {
        using I = std::underlying_type_t<Permissions>;
        return static_cast<Permissions>(~static_cast<I>(perms));
    }

    constexpr Permissions operator|(Permissions lhs, Permissions rhs) {
        using I = std::underlying_type_t<Permissions>;
        return static_cast<Permissions>(static_cast<I>(lhs) | static_cast<I>(rhs));
    }

    constexpr Permissions operator&(Permissions lhs, Permissions rhs) {
        using I = std::underlying_type_t<Permissions>;
        return static_cast<Permissions>(static_cast<I>(lhs) & static_cast<I>(rhs));
    }

    constexpr Permissions operator^(Permissions lhs, Permissions rhs) {
        using I = std::underlying_type_t<Permissions>;
        return static_cast<Permissions>(static_cast<I>(lhs) ^ static_cast<I>(rhs));
    }

    constexpr Permissions operator|=(Permissions& lhs, Permissions rhs) {
        return lhs = lhs | rhs;
    }

    constexpr Permissions operator&=(Permissions& lhs, Permissions rhs) {
        return lhs = lhs & rhs;
    }

    constexpr Permissions operator^=(Permissions& lhs, Permissions rhs) {
        return lhs = lhs ^ rhs;
    }

    std::string to_string(Permissions perms);
    Permissions perms_from_string(std::string_view str);

    std::istream& operator>>(std::istream& stream, Permissions& perms);
    std::ostream& operator<<(std::ostream& stream, Permissions perms);
}
