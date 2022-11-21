#pragma once
#include <string>

class Room {
   public:
    Room() = default;
    Room(const Room &) = delete;
    Room &operator=(const Room &) = delete;
    virtual ~Room();

    // send msg to a single user by id
    virtual void Send(const std::string &message, std::uint64_t id) = 0;

    // send message for all users in room except one (initiator)
    virtual void SendAllExcept(const std::string &message,
                               std::uint64_t id) = 0;
};