#include "guiobj.hpp"

const int CELL_SIZE = 100;
const int CELL_TITLE_SIZE = 20;


std::tuple<int, int> cell(std::string pos) {
    char letter = pos[0];
    char number = pos[1];
    int x = letter - 'a';
    int y = number - '1';
    return std::tuple{CELL_SIZE * x, CELL_SIZE * (7 - y)};
}

void setupCells(std::shared_ptr<GUIFactory> gui) {
    for (auto letter : "abcdefgh") {
        for (auto number : "12345678") {
            auto [x, y] = cell(std::string{letter, number});
            gui -> rect()
                    -> x(x)
                    -> y(y)
                    -> width(CELL_SIZE)
                    -> height(CELL_SIZE)
                    -> color((letter - 'a' + number - '1') % 2 ? 0x01796fffu : 0xf0fff0ffu)
                    -> create();
        }
    }
}

void setupCellTitles(std::shared_ptr<GUIFactory> gui) {
    for (auto letter : "abcdefgh") {
        char number = '1';
        auto [x, y] = cell(std::string{letter, number});
        gui -> text()
            -> x(x + CELL_SIZE - CELL_TITLE_SIZE)
            -> y(y + CELL_SIZE - CELL_TITLE_SIZE)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((letter - 'a' + number - '1') % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{letter})
            -> create();
    }

    for (auto number : "12345678") {
        char letter = 'a';
        auto [x, y] = cell(std::string{letter, number});
        gui -> text()
            -> x(x)
            -> y(y)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((letter - 'a' + number - '1') % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{number})
            -> create();
    }
}

void setupPawns(std::shared_ptr<GUIFactory> gui) {
    char number = '7';
    for (auto letter : "abcdefgh") {
        auto [x, y] = cell(std::string{letter, number});
        gui -> sprite()
            -> x(x)
            -> y(y)
            -> image("../figures.png")
            -> frame(1000, 200, 200, 200)
            -> scale(CELL_SIZE / 200.0)
            -> create();
    }

    number = '2';
    for (auto letter : "abcdefgh") {
        auto [x, y] = cell(std::string{letter, number});
        gui -> sprite()
            -> x(x)
            -> y(y)
            -> image("../figures.png")
            -> frame(1000, 0, 200, 200)
            -> scale(CELL_SIZE / 200.0)
            -> create();
    }
}

void setupKnights(std::shared_ptr<GUIFactory> gui) {

}

void setupFigures(std::shared_ptr<GUIFactory> gui) {
    setupPawns(gui);

//    for (int i = 0; i < 8; ++i) {
//        int row = 6;
//        int col = i;
//        gui -> sprite()
//            -> x(CELL_SIZE * col)
//            -> y(CELL_SIZE * row)
//            -> image("../figures.png")
//            -> frame(1000, 0, 200, 200)
//            -> scale(CELL_SIZE / 200.0)
//            -> create();
//    }
//
//    for (int i = 0; i < 8; ++i) {
//        int row = 6;
//        int col = i;
//        gui -> sprite()
//                -> x(CELL_SIZE * col)
//                -> y(CELL_SIZE * row)
//                -> image("../figures.png")
//                -> frame(1000, 0, 200, 200)
//                -> scale(CELL_SIZE / 200.0)
//                -> create();
//    }
//
//    for (int i = 0; i < 8; ++i) {
//        int row = 1;
//        int col = i;
//        gui -> sprite()
//                -> x(CELL_SIZE * col)
//                -> y(CELL_SIZE * row)
//                -> image("../figures.png")
//                -> frame(0, 0, 200, 200)
//                -> scale(CELL_SIZE / 200.0)
//                -> create();
//    }
}

void setup(std::shared_ptr<GUIFactory> gui) {
    setupCells(gui);
    setupCellTitles(gui);
    setupFigures(gui);
}