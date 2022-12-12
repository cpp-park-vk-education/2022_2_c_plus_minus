#include "Rock.hpp"

Rock::Rock(figure_color color) {
    Rock::isDead = 0;
    Rock::name = "Rock";
    Rock::color = color;
    Rock::FEN_name = (color == WHITE) ? "R" : "r";
}
