#pragma once

#include "Structures.hpp"
#include "string"
#include "Base.hpp"

struct GameState {
    bool is_started = false;
    bool is_finished = false;
    bool is_your_turn = false;
    bool is_in_room = false;

    std::string FEN_game_state = START_POSITION;
    std::string id;
    std::string enemy_id;
    figure_color color;
};
