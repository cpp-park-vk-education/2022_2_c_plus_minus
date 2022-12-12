#include "User.hpp"

bool User::IsEmpty() const noexcept {
    return !(this->id_) && this->nick_.empty();
}

void User::SetId(const uint64_t &id) noexcept { this->id_ = id; }
void User::SetNick(const std::string &nick) noexcept { this->nick_ = nick; }
const uint64_t &User::GetId() const noexcept { return this->id_; }
const std::string &User::GetNick() const noexcept { return this->nick_; }
