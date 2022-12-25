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
        if (mainMenu.haveClient(user.id)) {
            std::string roomId = user.id;
            std::string hostId = user.id;
            figure_color color = roomRequest->player_color;
            std::string roomName = roomRequest->name;
            mainMenu.room_manager_.createRoom(roomName, roomId, hostId, color,
                                              2);
            const User* mainMenuClient = mainMenu.removeClient(user.id);
            mainMenu.room_manager_.getRoom(roomId)->addClient(*mainMenuClient,
                                                              color);
            user.position = {Location::Room, roomId};
            roomResponse->status = 0;
            return;
        }
        roomResponse->status = 1;
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
        if (mainMenu.haveClient(user.id)) {
            std::string room_name = roomRequest->room_name;
            std::string roomId = mainMenu.room_manager_.getRoomId(room_name);
            if (mainMenu.room_manager_.haveRoom(roomId)) {
                const User* mainMenuClient = mainMenu.removeClient(user.id);
                Room* room = mainMenu.room_manager_.getRoom(roomId);
                figure_color color =
                    (room->getHostColor() == figure_color::WHITE
                         ? figure_color::BLACK
                         : figure_color::WHITE);
                room->addClient(*mainMenuClient, color);
                user.position = {Location::Room, roomId};

                roomResponse->enemy_name = room->getHost()->nickname;
                roomResponse->player_color = color;
                roomResponse->status = 0;
                EnterRoomResponse for_host;
                std::string str(user.nickname);
                for_host.enemy_name = user.nickname;
                for_host.player_color = color;
                for_host.status = 0;
                room->broadcast(user.nickname, QueryType::ENTER_ROOM,
                                for_host.toJSON());
                return;
            }
        }
        roomResponse->status = 1;
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
        if (user.position.first != Location::MainMenu) {
            if (mainMenu.room_manager_.getRoom(roomId)->haveClient(user.id)) {
                const User* roomClient =
                    mainMenu.room_manager_.getRoom(roomId)->removeClient(
                        user.id);
                if (mainMenu.room_manager_.getRoom(roomId)
                        ->getCurrentClientNumber() == 0) {
                    mainMenu.room_manager_.deleteRoom(roomId);
                }
                mainMenu.addClient(*roomClient);
                user.position = {Location::MainMenu, ""};

                roomResponse->status = 0;
                return;
            }
        }
        mainMenu.removeClient(user.id);
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
            if (user.id == room->getHostId()) {
                room->broadcast(user.id, QueryType::START_GAME);
                room->startGame();
                user.position = {Location::Game, roomId};
                roomResponse->status = 0;
                return;
            }
        }
        roomResponse->status = 1;
    }

    ~StartGameHandler() = default;
};