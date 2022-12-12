#pragma once

#include "Handler.hpp"
#include "RoomRequest.hpp"
#include "RoomResponse.hpp"

class CreateRoomHandler : public Handler {
   public:
    CreateRoomHandler() = default;

    void process(const Request* request, Response* response,
                 ClientData& clientData, BasicMenu& mainMenu) override {
        const CreateRoomRequest* roomRequest =
            dynamic_cast<const CreateRoomRequest*>(request);
        CreateRoomResponse* roomResponse =
            dynamic_cast<CreateRoomResponse*>(response);

        std::string roomId = clientData.id;
        std::string roomName = roomRequest->name;
        mainMenu.roomController.createRoom(roomName, roomId, 2);
        const ClientData* mainMenuClient = mainMenu.removeClient(clientData.id);
        mainMenu.roomController.getRoom(roomId)->addClient(*mainMenuClient);
        clientData.position = {Location::Room, roomId};

        roomResponse->status = 0;
    }

    ~CreateRoomHandler() = default;
};

class EnterRoomHandler : public Handler {
   public:
    EnterRoomHandler() = default;

    void process(const Request* request, Response* response,
                 ClientData& clientData, BasicMenu& mainMenu) override {
        const EnterRoomRequest* roomRequest =
            dynamic_cast<const EnterRoomRequest*>(request);
        EnterRoomResponse* roomResponse =
            dynamic_cast<EnterRoomResponse*>(response);
        std::string roomId = roomRequest->roomId;
        const ClientData* mainMenuClient = mainMenu.removeClient(clientData.id);
        Room* room = mainMenu.roomController.getRoom(roomId);
        room->addClient(*mainMenuClient);
        clientData.position = {Location::Room, roomId};

        if (room->getCurrentClientNumber() == room->getMaxClientNumber()) {
            sleep(0.2);
            room->broadcast(mainMenuClient->id, "StartGame");
            room->startGame();
        }

        roomResponse->status = 0;
    }

    ~EnterRoomHandler() = default;
};

class LeaveRoomHandler : public Handler {
   public:
    LeaveRoomHandler() = default;

    void process(const Request* request, Response* response,
                 ClientData& clientData, BasicMenu& mainMenu) override {
        LeaveRoomResponse* roomResponse =
            dynamic_cast<LeaveRoomResponse*>(response);

        std::string roomId = clientData.position.second;
        const ClientData* roomClient =
            mainMenu.roomController.getRoom(roomId)->removeClient(
                clientData.id);
        mainMenu.addClient(*roomClient);
        clientData.position = {Location::MainMenu, ""};

        roomResponse->status = 0;
    }

    ~LeaveRoomHandler() = default;
};

class GetRoomsHandler : public Handler {
   public:
    GetRoomsHandler() = default;

    void process(const Request* request, Response* response,
                 ClientData& clientData, BasicMenu& mainMenu) override {
        GetRoomsResponse* roomResponse =
            dynamic_cast<GetRoomsResponse*>(response);
        auto rooms = mainMenu.roomController.getAllRooms();
        roomResponse->rooms = rooms;
        roomResponse->status = 0;
    }

    ~GetRoomsHandler() = default;
};

class StartGameHandler : public Handler {
   public:
    StartGameHandler() = default;

    void process(const Request* request, Response* response,
                 ClientData& clientData, BasicMenu& mainMenu) override {
        const StartGameRequest* roomRequest =
            dynamic_cast<const StartGameRequest*>(request);
        StartGameResponse* roomResponse =
            dynamic_cast<StartGameResponse*>(response);

        std::string roomId = clientData.position.second;
        mainMenu.roomController.getRoom(roomId)->startGame();
        clientData.position = {Location::Game, roomId};

        roomResponse->status = 0;
    }

    ~StartGameHandler() = default;
};
