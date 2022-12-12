#pragma once
#include <string>

#include "Request.hpp"

struct CreateRoomRequest : public Request {
    std::string name;
    void parse(boost::json::object &requestData) override {
        name = requestData["roomName"].as_string().c_str();
        operation_result_ = true;
    }

    CreateRoomRequest(std::string roomName) { name = roomName; }
    CreateRoomRequest() = default;

    std::string toJSON() override {
        boost::json::object object({{"roomName", name}});
        return dataRequest("CreateRoom", object);
    }
};

struct EnterRoomRequest : public Request {
    std::string roomId;

    void parse(boost::json::object &requestData) override {
        //        std::cout << "parsing enter room";
        roomId = requestData["roomId"].as_string().c_str();
        operation_result_ = true;
    }

    EnterRoomRequest(std::string id) { roomId = id; }
    EnterRoomRequest() = default;

    std::string toJSON() override {
        boost::json::object object({{"roomId", roomId}});
        return dataRequest("EnterRoom", object);
    }
};

struct LeaveRoomRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest("LeaveRoom"); }
};

struct GetRoomsRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest("GetRooms"); }
};

struct StartGameRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest("StartGame"); }
};
