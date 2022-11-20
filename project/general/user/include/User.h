#pragma once

#include <cstdint>
#include <string>

class User {
public:
    User(uint64_t id);
    ~User() = default;
    uint64_t Id() const;
    std::string Name() const;
    void SetName(const std::string& name);
    std::uint64_t RoomId() const;
    void SetRoomId(const uint64_t& roomId);

private:
    std::uint64_t id_;
    std::string name_;
    std::uint64_t room_id_;
};
