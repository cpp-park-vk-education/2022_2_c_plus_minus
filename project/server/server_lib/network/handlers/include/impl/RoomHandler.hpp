#pragma once

#include "Handler.hpp"
#include "RoomRequest.hpp"
#include "RoomResponse.hpp"

class CreateRoomHandler : public Handler {
   public:
    CreateRoomHandler() = default;

    void process(const Request* request, Response* response, User& user,
                 BasicMenu& mainMenu) override {
        const CreateRoomRequest* roomRequest =
            dynamic_cast<const CreateRoomRequest*>(request);
        CreateRoomResponse* roomResponse =
            dynamic_cast<CreateRoomResponse*>(response);

        std::string roomId = user.id;
        std::string hostId = user.id;
        figure_color color = roomRequest->player_color;
        std::string roomName = roomRequest->name;
        mainMenu.room_manager_.createRoom(roomName, roomId, hostId, color, 2);
        const User* mainMenuClient = mainMenu.removeClient(user.id);
        mainMenu.room_manager_.getRoom(roomId)->addClient(*mainMenuClient, color);
        user.position = {Location::Room, roomId};
        roomResponse->status = 0;
    }

    ~CreateRoomHandler() = default;
};

class EnterRoomHandler : public Handler {
   public:
    EnterRoomHandler() = default;

    void process(const Request* request, Response* response, User& user,
                 BasicMenu& mainMenu) override {
        const EnterRoomRequest* roomRequest =
            dynamic_cast<const EnterRoomRequest*>(request);
        EnterRoomResponse* roomResponse =
            dynamic_cast<EnterRoomResponse*>(response);
        std::string roomId = roomRequest->roomId;
        const User* mainMenuClient = mainMenu.removeClient(user.id);
        Room* room = mainMenu.room_manager_.getRoom(roomId);
        figure_color color = (room->getHostColor() == figure_color::WHITE ? figure_color::BLACK
                                                                                : figure_color::WHITE);
        room->addClient(*mainMenuClient, color);
        user.position = {Location::Room, roomId};

        roomResponse->enemy_id = room->getHostId();
        roomResponse->player_color = color;
//        if (room->getCurrentClientNumber() == room->getMaxClientNumber()) {
//            room->broadcast(mainMenuClient->id, QueryType::START_GAME);
//            room->startGame();
//            room->write(mainMenuClient->id, QueryType::START_GAME);
//        }
        roomResponse->status = 0;
    }

    ~EnterRoomHandler() = default;
};

class LeaveRoomHandler : public Handler {
   public:
    LeaveRoomHandler() = default;

    void process(const Request* request, Response* response, User& user,
                 BasicMenu& mainMenu) override {
        LeaveRoomResponse* roomResponse =
            dynamic_cast<LeaveRoomResponse*>(response);

        std::string roomId = user.position.second;
        const User* roomClient =
            mainMenu.room_manager_.getRoom(roomId)->removeClient(user.id);
        mainMenu.addClient(*roomClient);
        user.position = {Location::MainMenu, ""};

        roomResponse->status = 0;
    }

    ~LeaveRoomHandler() = default;
};

class GetRoomsHandler : public Handler {
   public:
    GetRoomsHandler() = default;

    void process(const Request* request, Response* response, User& user,
                 BasicMenu& mainMenu) override {
        GetRoomsResponse* roomResponse =
            dynamic_cast<GetRoomsResponse*>(response);
        auto rooms = mainMenu.room_manager_.getAllRooms();
        roomResponse->rooms = rooms;
        roomResponse->status = 0;
    }

    ~GetRoomsHandler() = default;
};

class StartGameHandler : public Handler {
   public:
    StartGameHandler() = default;

    void process(const Request* request, Response* response, User& user,
                 BasicMenu& mainMenu) override {
        StartGameResponse* roomResponse =
                dynamic_cast<StartGameResponse*>(response);
        std::string roomId = user.position.second;
        auto room = mainMenu.room_manager_.getRoom(roomId);
        if (room->getCurrentClientNumber() == room->getMaxClientNumber()) {
            if (user.id == room->getHostId() ) {
                room->broadcast(user.id, QueryType::START_GAME);
                room->startGame();
                user.position = {Location::Game, roomId};
                roomResponse->status = 0;
            }
            return;
        }
        roomResponse->status = 1;
    }

    ~StartGameHandler() = default;
};