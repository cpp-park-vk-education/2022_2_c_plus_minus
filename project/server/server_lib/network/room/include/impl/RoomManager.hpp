#pragma once
#include <map>

#include "Room.hpp"

class RoomManager {
   public:
    RoomManager() = default;

    Room* getRoom(std::string roomId);
    bool haveRoom(std::string id);
    std::map<std::string, Room*> getAllRooms();
    void createRoom(std::string name, std::string roomId,
                                 std::string host_id, const figure_color& color,
                                 unsigned int maxClientNumber);
    void deleteRoom(std::string roomId);
    void deleteFromRoom(std::string user_id);

    ~RoomManager() = default;

   private:
    std::map<std::string, Room*> rooms_;
};
