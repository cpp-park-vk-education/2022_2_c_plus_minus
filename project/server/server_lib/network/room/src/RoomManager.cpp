#include "RoomManager.hpp"

Room* RoomManager::getRoom(std::string roomId) {
    if (haveRoom(roomId)) {
        return rooms_[roomId];
    } else {
        return nullptr;
    }
}

bool RoomManager::haveRoom(std::string id) {
    return rooms_.find(id) != rooms_.end();
}

std::map<std::string, Room*> RoomManager::getAllRooms() { return rooms_; }

void RoomManager::createRoom(std::string name, std::string roomId,
                             unsigned int maxClientNumber) {
    rooms_.insert({roomId, new Room(roomId, name, maxClientNumber)});
}

void RoomManager::deleteRoom(std::string roomId) {
    delete getRoom(roomId);
    rooms_.erase(roomId);
}
