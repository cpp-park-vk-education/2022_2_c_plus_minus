#pragma once

#include "Structures.hpp"
#include "string"
#include "Base.hpp"

struct GameState {
    std::atomic_bool is_started = false;
    std::atomic_bool is_finished = false;
    std::atomic_bool is_your_turn = false;
    std::atomic_bool is_in_room = false;
    std::atomic_bool is_room_full = false;

    std::string FEN_game_state = START_POSITION;
    std::string id;
    std::string enemy_id;
    figure_color color;
    move_status last_move;
    std::queue<std::string> movesChan;
};
