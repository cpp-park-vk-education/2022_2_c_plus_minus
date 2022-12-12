#pragma once

#include "Figure.hpp"

class Pawn : public Figure {
   public:
    bool enPassant = false;
    Pawn(figure_color color);
};