#include "King.hpp"

King::King(figure_color color) {
    King::isDead = 0;
    King::name = "King";
    King::color = color;
    King::FEN_name = (color == WHITE) ? "K" : "k";
}
