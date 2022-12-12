#pragma once
#include "Figure.hpp"

class King : public Figure {
   public:
    King(figure_color color);
    int isCheked = 0;
    int moved = 0;
    int cast_k = 0;  // рокировка со стороны короля
    int cast_q = 0;  // рокировка со стороны ферзя
    int mateCheck(Figure **table);
    int checkMateCheck(Figure **table);
};
