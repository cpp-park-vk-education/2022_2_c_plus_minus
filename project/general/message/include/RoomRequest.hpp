#pragma once
#include <string>

#include "Request.hpp"

struct CreateRoomRequest : public Request {
    std::string name;
    // host chooses color
    figure_color player_color;
    void parse(boost::json::object &requestData) override {
        name = requestData["roomName"].as_string().c_str();
        player_color = figure_color(requestData["color"].as_int64());
        operation_result_ = true;
    }

    CreateRoomRequest(const std::string &roomName, figure_color pColor)
        : name(roomName), player_color(pColor) {}
    CreateRoomRequest() = default;

    std::string toJSON() override {
        boost::json::object object(
            {{"roomName", name}, {"color", int(player_color)}});
        return dataRequest(QueryType::CREATE_ROOM, object);
    }
};

struct EnterRoomRequest : public Request {
    std::string room_name;

    void parse(boost::json::object &requestData) override {
        room_name = requestData["room-name"].as_string().c_str();
        operation_result_ = true;
    }

    EnterRoomRequest(std::string name) { room_name = name; }
    EnterRoomRequest() = default;

    std::string toJSON() override {
        boost::json::object object({{"room-name", room_name}});
        return dataRequest(QueryType::ENTER_ROOM, object);
    }
};

struct LeaveRoomRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override {
        return basicRequest(QueryType::LEAVE_ROOM);
    }
};

struct GetRoomsRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override { return basicRequest(QueryType::GET_ROOMS); }
};

struct StartGameRequest : public Request {
    void parse(boost::json::object &requestData) override { return; }
    std::string toJSON() override {
        return basicRequest(QueryType::START_GAME);
    }
};
