#pragma once

#include "User.h"

std::uint64_t User::Id() const {
    return id_;
}

std::string User::Name() const {
    return name_;
}

void User::SetName(const std::string& name) {
    name_ = name;
}

std::uint64_t User::RoomId() const {
    return room_id_;
}

void User::SetRoomId(const uint64_t& roomId) {
    room_id_ = roomId;
}