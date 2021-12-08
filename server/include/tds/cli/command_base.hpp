#pragma once

#include <ostream>

namespace tds::cli {
    class CommandBase {
    public:
        CommandBase(const CommandBase&) = delete;
        CommandBase& operator=(const CommandBase&) = delete;

    protected:
        CommandBase();

        void set_error();
        int get_status() const noexcept;
        bool is_ok() const noexcept;
        std::ostream& log_error();

    private:
        int m_status;
    };
}
