#pragma once

#include "tds/cli/command.hpp"

#include <functional>
#include <span>
#include <string_view>

namespace tds::cli {
    template<typename T>
    class CommandInterface : public CommandBase {
    public:
        using Derived = T;

        CommandInterface() = default;

        static consteval std::string_view get_name() {
            return Derived::name;
        }

        int operator()(std::span<const std::string_view> args) {
            Derived& derived = *get_derived();
            derived.parse_arguments(args);
            derived.execute();
            return get_status();
        }

        template<std::invocable<T>... Steps>
        void execute_steps(Steps&&... steps) {
            if(is_ok()) {
                execute_steps_impl(std::forward<Steps>(steps)...);
            }
        }

    private:
        template<typename Head, typename... Tail>
        void execute_steps_impl(Head&& head, Tail&&... tail) {
            std::invoke(std::forward<Head>(head), get_derived());

            if constexpr(sizeof...(Tail) > 0) {
                if(is_ok()) {
                    execute_steps(std::forward<Tail>(tail)...);
                }
            }
        }

        Derived* get_derived() noexcept requires Command<Derived> {
            return static_cast<Derived*>(this);
        }
    };
}
