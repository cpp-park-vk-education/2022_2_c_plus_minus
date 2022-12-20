#pragma once

#include <stdlib.h>

#include <string>
#include <vector>

#include "Table.hpp"

class GameSession {
   public:
    const std::string id;
    const std::string whitePlayerId;
    const std::string blackPlayerId;
    int turns = 0;
    ChessTable table;
    int ended = 0;
    int beginned = 1;

    move_status gameResult = MOVE_OK;
    GameSession(const std::string &id, const std::string &whitePlayerId,
                const std::string &blackPlayerId,
                const std::string &FEN = START_POSITION);
    return_after_move makeMove(std::string move, const std::string &player_id);
    bool end_game(move_status why);
    ~GameSession();

   private:
    void create_error_response(return_after_move &result) const;
};
