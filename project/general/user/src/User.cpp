#include "User.hpp"

bool User::IsEmpty() const noexcept {
    return !(this->id_) && this->password_.empty() && this->nick_.empty();
}

void User::SetId(const uint64_t &id) noexcept { this->id_ = id; }

void User::SetPassword(const std::string &pw) noexcept { this->password_ = pw; }

void User::SetNick(const std::string &nick) noexcept { this->nick_ = nick; }

uint64_t &User::GetId() noexcept { return this->id_; }

std::string &User::GetPassword() noexcept { return this->password_; }

std::string &User::GetNick() noexcept { return this->nick_; }
