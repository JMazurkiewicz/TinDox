#include "tds/user/user_record_data.hpp"

#include "tds/linux/hash.hpp"

#include <ranges>
#include <stdexcept>

namespace tds::user {
    UserRecordData::UserRecordData(std::string username, std::string password_hash, Permissions perms)
        : m_username{std::move(username)}
        , m_password_hash{std::move(password_hash)}
        , m_perms{perms} { }

    std::string_view UserRecordData::get_username() const noexcept {
        return m_username;
    }

    std::string_view UserRecordData::get_password_hash() const noexcept {
        return m_password_hash;
    }

    Permissions UserRecordData::get_permissions() const noexcept {
        return m_perms;
    }

    bool UserRecordData::check_password(const std::string& password) const {
        return check_password(password.c_str());
    }

    bool UserRecordData::check_password(const char* password) const {
        return m_password_hash == linux::hash(password);
    }

    UserRecordData make_user_record_data(std::string_view str) {
        /// @todo This code works before P2210R2
        auto splitted = str | std::views::split(':');
        if(std::ranges::distance(splitted) != 3) {
            throw std::runtime_error{"User record requires exactly three fields: 'username:password_hash:perms'"};
        }

        auto it = splitted.begin();
        auto common_username = *it | std::views::common;
        std::string username(common_username.begin(), common_username.end());

        ++it;
        auto common_password = *it | std::views::common;
        std::string password(common_password.begin(), common_password.end());

        ++it;
        auto common_perms_str = *it | std::views::common;
        const std::string perms_str(common_perms_str.begin(), common_perms_str.end());

        return UserRecordData{std::move(username), std::move(password), perms_from_string(perms_str)};
    }
}
