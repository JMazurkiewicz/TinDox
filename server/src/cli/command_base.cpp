#include "tds/cli/command_base.hpp"

#include <iostream>

namespace tds::cli {
    void CommandBase::set_error() {
        m_status = 1;
    }

    int CommandBase::get_status() const noexcept {
        return m_status;
    }

    bool CommandBase::is_ok() const noexcept {
        return get_status() == 0;
    }

    std::ostream& CommandBase::log_error() {
        m_status = 1;
        return std::cerr;
    }
}
