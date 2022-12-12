#pragma once

#include "Figure.hpp"

class Bishop : public Figure {
   public:
    int wasPawn = 0;
    Bishop(figure_color color);
};