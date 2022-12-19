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
        return dataRequest(QueryType::CREATE_ROOM, object);
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
        return dataRequest(QueryType::ENTER_ROOM, object);
    }
};

struct LeaveRoomRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest(QueryType::LEAVE_ROOM); }
};

struct GetRoomsRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest(QueryType::GET_ROOMS); }
//    std::string toJSON() override { return basicRequest(QueryType::GET_ROOMS); }
};

struct StartGameRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest(QueryType::START_GAME); }
};
