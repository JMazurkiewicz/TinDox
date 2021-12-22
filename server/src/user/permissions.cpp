#include "tds/user/permissions.hpp"

#include <array>
#include <stdexcept>

#include <fmt/core.h>

namespace tds::user {
    namespace {
        constexpr inline std::array permission_list = {
            Permissions::travel, Permissions::write,    Permissions::copy,
            Permissions::move,   Permissions::download, Permissions::upload,
        };

        constexpr inline std::string_view permission_symbols = "twcmdu";
    }

    std::string to_string(Permissions perms) {
        std::string result;
        for(int i = 0; Permissions perm : permission_list) {
            if((perms & perm) != Permissions{}) {
                result += permission_symbols[i];
            }

            ++i;
        }
        return result;
    }

    Permissions perms_from_string(std::string_view str) {
        Permissions result{};
        for(char perm_symbol : str) {
            if(auto pos = permission_symbols.find(perm_symbol); pos != std::string_view::npos) {
                result |= permission_list[pos];
            } else {
                throw std::runtime_error{fmt::format("Invalid permission symbol '{}'", perm_symbol)};
            }
        }
        return result;
    }

    std::istream& operator>>(std::istream& stream, Permissions& perms) {
        std::string str;
        stream >> str;
        perms = perms_from_string(str);
        return stream;
    }

    std::ostream& operator<<(std::ostream& stream, Permissions perms) {
        return stream << to_string(perms);
    }
}
