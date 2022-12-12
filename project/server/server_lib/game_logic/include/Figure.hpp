#pragma once

#include "Structures.hpp"

class Figure  // имя класса
{
   public:
    figure_color color;
    std::string name;
    std::string FEN_name;
    int isDead;
    Figure(const Figure &figure) = default;
    Figure() = default;
    ~Figure() = default;
};