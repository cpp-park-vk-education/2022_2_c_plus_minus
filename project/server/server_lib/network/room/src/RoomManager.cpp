#include "RoomManager.hpp"

std::string RoomManager::getRoomId(std::string name) {
    for (const auto& room : rooms_){
        if (room.second->getName() == name){
            return room.first;
        }
    }
    return "";
}

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

std::map<std::string, RoomData> RoomManager::getAllRooms() {
    std::map<std::string, RoomData> room_data_map;
    for (const auto& room : rooms_){
        room_data_map[room.first] = RoomData{
                room.second->getName(),
                room.second->getHost()->nickname,
                room.first,
                room.second->getHostColor()};
    }
    return room_data_map;
}

void RoomManager::createRoom(std::string name, std::string roomId,
                             std::string host_id, const figure_color& color,
                             unsigned int maxClientNumber) {
    rooms_.insert({roomId, new Room(roomId, name, host_id, color, maxClientNumber)});
}

void RoomManager::deleteRoom(std::string roomId) {
    delete getRoom(roomId);
    rooms_.erase(roomId);
}

void RoomManager::deleteFromRoom(std::string user_id) {
    for (auto room : rooms_) {
        if (room.second->haveClient(user_id)) {
            room.second->removeClient(user_id);
            if (room.second->getCurrentClientNumber() == 0) {
                deleteRoom(room.second->getId());
                break;
            }
        }
    }
}