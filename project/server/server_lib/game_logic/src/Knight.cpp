#include "Knight.hpp"

Knight::Knight(figure_color color) {
    Knight::isDead = 0;
    Knight::name = "Knight";
    Knight::color = color;
    Knight::FEN_name = (color == WHITE) ? "N" : "n";
}
