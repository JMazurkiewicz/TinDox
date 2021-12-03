#pragma once

#include "tds/cli/command.hpp"

#include <span>
#include <string_view>

namespace tds::cli {
    template<typename T>
    class CommandInterface : public CommandBase {
    public:
        using Derived = T;

        static consteval std::string_view get_name() {
            return Derived::name;
        }

        int execute(std::span<const std::string_view> args) {
            return get_derived()->do_execute(args);
        }

    private:
        Derived* get_derived() noexcept requires Command<Derived> {
            return static_cast<Derived*>(this);
        }
    };
}
