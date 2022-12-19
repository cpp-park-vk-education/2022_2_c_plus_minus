#pragma once

#include "Response.hpp"
#include "string"

//
#define START_POSITION "rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"
#define BOARD_SIZE 64

enum figure_color { WHITE = 1, BLACK = -1 };

struct coords {
    std::string str;
    int index;
    int x;
    int y;
};

struct return_after_move {
    move_status moveStatus;
    std::string table_fen;
    int move_from;
    int move_to;
};

coords data_to_coords(char letter, char number);
int str_to_index(char letter, char number);
coords data_to_coords(int index);
coords data_to_coords(std::string input);
std::string index_to_str(const int index);