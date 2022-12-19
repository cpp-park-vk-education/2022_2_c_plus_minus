#pragma once

#include "GameRequest.hpp"
#include "GameResponse.hpp"
#include "GameSession.hpp"
#include "Handler.hpp"

class AuthHandler : public Handler {
   public:
    AuthHandler() = default;

    void process(const Request* request, Response* response,
                 User& user, BasicMenu& menu) override {
        const AuthRequest* auth_request =
            dynamic_cast<const AuthRequest*>(request);
        AuthResponse* auth_response = dynamic_cast<AuthResponse*>(response);
        user.nickname = auth_request->nick;
        auth_response->status = 0;
        auth_response->message = "Success Auth";
    }

    ~AuthHandler() = default;
};

class GameHandler : public Handler {
   public:
    GameHandler() = default;

    void process(const Request* request, Response* response,
                 User& user, BasicMenu& menu) override {
        const MoveFigureRequest* game_request =
            dynamic_cast<const MoveFigureRequest*>(request);
        GameResponse* gameResponse = dynamic_cast<GameResponse*>(response);

        Room* clientGame =
            menu.room_manager_.getRoom(user.position.second);
        std::string move = game_request->move;
        return_after_move moveResult =
            clientGame->makeAction(user.id, move);

        gameResponse->moveStatus = moveResult.moveStatus;
        gameResponse->tableFEN = moveResult.table_fen;
        gameResponse->moveFrom = moveResult.move_from;
        gameResponse->moveTo = moveResult.move_to;

        if (moveResult.moveStatus != MOVE_ERROR) {
            clientGame->broadcast(user.id, QueryType::MOVE_FIGURE, gameResponse->toJSON());
        }
    }

    ~GameHandler() = default;
};
