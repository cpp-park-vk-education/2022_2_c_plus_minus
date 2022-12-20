#pragma once

#include "Response.hpp"
#include "Room.hpp"

struct CreateRoomResponse : public Response {
    int status;
    CreateRoomResponse() = default;
    void parse(const std::string &requestData) override {}
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
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object(
            {{"status", status}, {"game_started", game_started}, {"id", id}});

        return boost::json::serialize(object);
    }
};

struct EnterRoomResponse : public Response {
    int status = 0;
    figure_color player_color;
    std::string enemy_id;
    EnterRoomResponse() = default;
    EnterRoomResponse(int st, figure_color pColor, std::string id)
        : status(st), player_color(pColor), enemy_id(id) {}

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
        enemy_id = boost::json::value_to<std::string>(parsedData.at("enemy-id"));
    }

    std::string toJSON() override {
        boost::json::object object(
            {{"status", status}, {"color", int(player_color)}, {"enemy-id", enemy_id}});

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
    std::map<std::string, Room *> rooms;

    GetRoomsResponse() = default;
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object({{"status", status}});
        boost::json::array arr;
        for (auto el : rooms) {
            boost::json::object room(
                {{el.first.data(),
                  {el.second->getName(), el.second->getHost()->nickname}}});
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
