#pragma once

#include "GameRequest.hpp"
#include "GameResponse.hpp"
#include "GameSession.hpp"
#include "Handler.hpp"

class GameHandler : public Handler {
   public:
    GameHandler() = default;

    void process(const Request* request, Response* response,
                 ClientData& clientData, BasicMenu& menu) override {
        const MoveFigureRequest* game_request =
            dynamic_cast<const MoveFigureRequest*>(request);
        GameResponse* gameResponse = dynamic_cast<GameResponse*>(response);

        Game clientGame =
            menu.roomController.getRoom(clientData.position.second)->getGame();
        std::string move = game_request->move;
        return_after_move moveResult =
            clientGame.makeAction(clientData.id, move);

        gameResponse->moveStatus = moveResult.moveStatus;
        gameResponse->tableFEN = moveResult.table_fen;
        gameResponse->moveFrom = moveResult.move_from;
        gameResponse->moveTo = moveResult.move_to;

        if (moveResult.moveStatus != MOVE_ERROR) {
            clientGame.broadcast(clientData.id, gameResponse->toJSON());
        }
    }

    ~GameHandler() = default;
};
