#pragma once

#include "RoomImpl.hpp"
class ChessEngine;

// class for managing a game between users
class GameRoom : public RoomImpl {
    void Send(const std::string& message, std::uint64_t id) override;
    // send message for all users in room except one (initiator)
    void SendAllExcept(const std::string& message, std::uint64_t id) override;

private:
    const std::uint64_t id_ = 0;
    std::uint64_t users_count = 0;
    std::uint64_t users_max_ = 0;
    std::string name_;
    // store sessions of room users in map
    std::map<std::uint64_t, std::shared_ptr<SessionImpl>> sessions_;
    // multiple users may concurrently access to sessions, need mutex
    mutable std::mutex mutex_;
//    ChessEngine engine_;
};
