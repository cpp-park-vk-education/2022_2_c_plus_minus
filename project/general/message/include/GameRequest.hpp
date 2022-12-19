#pragma once
#include "Request.hpp"

struct AuthRequest : public Request {
    std::string nick;
    void parse(boost::json::object& requestData) override {
        nick = requestData["nick"].as_string().c_str();
    }

    std::string toJSON() override {
        boost::json::object object({{"nick", nick}});
        return dataRequest(QueryType::AUTHORISE, object);
    }
    AuthRequest(){
        nick = "Unidentified turtle";
    }
    AuthRequest(std::string nickname) : nick(nickname) {}
};

struct MoveFigureRequest : public Request {
    std::string move;

    void parse(boost::json::object& requestData) override {
        move = requestData["moveStr"].as_string().c_str();
        if (move.size() == 4 && isValidCoord(move.substr(0, 2)) &&
            isValidCoord(move.substr(2, 2))) {
            operation_result_ = true;
        } else {
            operation_result_ = false;
        }
    }

    std::string toJSON() override {
        boost::json::object object({{"moveStr", move}});
        return dataRequest(QueryType::MOVE_FIGURE, object);
    }
    MoveFigureRequest() = default;
    MoveFigureRequest(std::string fromTo) : move(fromTo) {}

   private:
    bool isValidCoord(const std::string& coord) {
        return (coord.size() == 2 && isBoardLetter(coord[0]) &&
                isBoardDigit(coord[1]));
    }

    bool isBoardDigit(char c) { return ('1' <= c && c <= '8'); }

    bool isBoardLetter(char c) { return ('A' <= c && c <= 'H'); }
};
