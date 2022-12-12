#include "Structures.hpp"

std::string index_to_str(const int index) {
    std::string coord = "";
    coord += index % 8 + 'A';
    coord += index / 8 + '1';
    return coord;
}

coords data_to_coords(std::string input) {
    coords result;
    result.str = input;
    int index = (input.at(0) - 'A') + (input.at(1) - '0') * 8;
    if (!((index >= 0) && (index < BOARD_SIZE))) {
        result.index = -1;
        result.x = -1;
        result.y = -1;
        return result;
    }
    result.index = index;
    result.x = index % 8;
    result.y = index / 8;
    return result;
}

coords data_to_coords(int index) {
    coords result;
    result.str = index_to_str(index);
    if (!((index >= 0) && (index < BOARD_SIZE))) {
        result.index = -1;
        result.x = -1;
        result.y = -1;
        return result;
    }
    result.index = index;
    result.x = index % 8;
    result.y = index / 8;
    return result;
}

coords data_to_coords(char letter, char number) {
    coords result;
    int index = (letter - 'A') + (number - '1') * 8;
    result.str = letter + number;
    if (!((index >= 0) && (index < BOARD_SIZE))) {
        result.index = -1;
        result.x = -1;
        result.y = -1;
        return result;
    }
    result.index = index;
    result.x = index % 8;
    result.y = index / 8;
    return result;
}

int str_to_index(char letter, char number) {
    // std::cout<<letter<<number<<std::endl;//DEBUG

    int index = (letter - 'A') + (number - '1') * 8;
    // std::cout<<index<<std::endl;//DEBUG
    if (((index >= 0) && (index < BOARD_SIZE))) {
        return index;
    }
    return -1;
}