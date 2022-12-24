#pragma once

#include "Response.hpp"
#include "Room.hpp"

struct CreateRoomResponse : public Response {
    int status;
    CreateRoomResponse() = default;
    void parse(const std::string &requestData) override {
        boost::json::error_code ec;
        boost::json::value parsedData = boost::json::parse(requestData, ec);
        if (ec) {
            operation_result_ = false;
            return;
        }
        operation_result_ = true;

        status = parsedData.at("status").to_number<int>();
    }
    std::string toJSON() override {
        boost::json::object object({{"status", status}});

        return boost::json::serialize(object);
    }
};

struct OnEnterRoomResponse : public Response {
    int status;
    bool game_started = 0;
    std::string id;

    OnEnterRoomResponse() = default;
    void parse(const std::string &requestData) override {
        boost::json::error_code ec;
        boost::json::value parsedData = boost::json::parse(requestData, ec);
        if (ec) {
            operation_result_ = false;
            return;
        }
        operation_result_ = true;

        status = parsedData.at("status").to_number<int>();
    }
    std::string toJSON() override {
        boost::json::object object(
            {{"status", status}, {"game_started", game_started}, {"id", id}});

        return boost::json::serialize(object);
    }
};

struct EnterRoomResponse : public Response {
    int status = 0;
    figure_color player_color;
    std::string enemy_name;
    EnterRoomResponse() = default;
    EnterRoomResponse(int st, figure_color pColor, std::string name)
        : status(st), player_color(pColor), enemy_name(name) {}

    void parse(const std::string &requestData) override {
        boost::json::error_code ec;
        boost::json::value parsedData = boost::json::parse(requestData, ec);
        if (ec) {
            operation_result_ = false;
            return;
        }
        operation_result_ = true;

        status = parsedData.at("status").to_number<int>();
        player_color = figure_color(parsedData.at("color").to_number<int>());
        enemy_name= boost::json::value_to<std::string>(parsedData.at("enemy-name"));
    }

    std::string toJSON() override {
        boost::json::object object(
            {{"status", status}, {"color", int(player_color)}, {"enemy-name", enemy_name}});

        return boost::json::serialize(object);
    }
};

struct LeaveRoomResponse : public Response {
    int status;

    LeaveRoomResponse() = default;
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object({{"status", status}});
        return boost::json::serialize(object);
    }
};

struct GetRoomsResponse : public Response {
    int status;
    std::map<std::string, RoomData> rooms;

    GetRoomsResponse() = default;
    void parse(const std::string &responseData) override {
        boost::json::error_code ec;
        boost::json::value parsedData = boost::json::parse(responseData, ec);
        if (ec) {
            operation_result_ = false;
            return;
        }
        operation_result_ = true;

        status = parsedData.at("status").to_number<int>();
        auto rooms_array = parsedData.at("rooms").as_array();
        for (auto room : rooms_array){
            rooms[room.at("id").as_string().c_str()] = RoomData{
                    room.at("room-name").as_string().c_str(),
                    room.at("host-nick").as_string().c_str(),
                    room.at("id").as_string().c_str(),
                    figure_color(room.at("host-color").as_int64()),
            } ;
        }
    }

    std::string toJSON() override {
        boost::json::object object({{"status", status}});
        boost::json::array arr;
        for (auto el : rooms) {
            boost::json::object room({
                  {"id", el.first.data()},
                  {"room-name", el.second.room_name},
                  {"host-nick", el.second.host_nick},
                  {"host-color", int(el.second.host_color)}});
            arr.push_back(room);
        }
        object["rooms"] = arr;
        return boost::json::serialize(object);
    }
};

struct StartGameResponse : public Response {
    int status;
    void parse(const std::string &requestData) override {}
    StartGameResponse() = default;
    std::string toJSON() override {
        boost::json::object object({{"status", status}});

        return boost::json::serialize(object);
    }
};
