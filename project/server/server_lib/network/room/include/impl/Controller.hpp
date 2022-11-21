#pragma once

#include <limits>
#include <memory>
#include <unordered_map>
#include <vector>

#include "GameRoomImpl.hpp"
#include "RoomImpl.hpp"

class Session;

class Controller final {
   public:
    Controller();

    ~Controller();

    const Room& GetRoom(std::uint64_t id) const;
    const GameRoom& GetGameRoom(std::uint64_t id) const;
    std::vector<std::shared_ptr<Room>> GetAllRooms() const;

    bool NewSession(const std::shared_ptr<Session>& session);
    void DeleteSession(const std::shared_ptr<Session>& session);
    std::uint64_t NewRoom(std::string name, std::uint64_t maxUserNum);
    void DeleteRoom(std::uint64_t roomId);

    // ( 1st player session id and 2nd session player id ) --> vector
    std::vector<uint64_t> StartGame(std::uint64_t id);

    std::int64_t Move(const std::shared_ptr<Session>& session,
                          std::uint64_t figure_id, std::uint64_t pos_x,
                          std::uint64_t pos_y);

    void Send(const std::string& message, std::uint64_t id);
    void SendAllExcept(const std::string& message, std::uint64_t roomId,
                   std::uint64_t userId);


    static constexpr std::uint64_t base_room_id = 0;
    static constexpr std::uint64_t game_room_id = 0;

   private:
    std::map<std::uint64_t, std::shared_ptr<GameRoom>> rooms_;

    std::shared_ptr<Room> base_room_ = std::make_shared<RoomImpl>(
        base_room_id, std::numeric_limits<std::uint64_t>::max(), "Basic Room");

    mutable std::mutex mutex_;
};
