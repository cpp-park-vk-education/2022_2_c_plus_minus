#include <SFML/Graphics.hpp>
#include "guiobj.hpp"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = WINDOW_WIDTH / 8;
const int CELL_TITLE_SIZE = 20;

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "chess");
    std::shared_ptr<GUIFactory> gui(new SFMLGUIFactory(&window));

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            gui -> rect()
                -> x(col * CELL_SIZE)
                -> y(row * CELL_SIZE)
                -> width(CELL_SIZE)
                -> height(CELL_SIZE)
                -> color((row + col) % 2 ? sf::Color::Black.toInteger() : sf::Color::White.toInteger())
                -> create();
        }
    }

    for (int i = 0; i < 8; ++i) {
        int row = 7;
        int col = i;
        gui -> text()
            -> x(CELL_SIZE * (i + 1) - CELL_TITLE_SIZE)
            -> y(CELL_SIZE * 8 - CELL_TITLE_SIZE)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((row + col) % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{"abcdefgh"[i]})
            -> create();
    }

    for (int i = 0; i < 8; ++i) {
        int row = i;
        int col = 0;
        gui -> text()
            -> x(0)
            -> y(CELL_SIZE * i)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((row + col) % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{"12345678"[i]})
            -> create();
    }

    for (int i = 0; i < 8; ++i) {
        int row = 6;
        int col = i;
        gui -> sprite()
            -> x(CELL_SIZE * col)
            -> y(CELL_SIZE * row)
            -> image("../figures.png")
            -> frame(80, 160, 100, 100)
            -> mask(sf::Color(78, 144, 79).toInteger())
            -> create();
    }

//    gui -> sprite()
//        -> x(0)
//        -> y(0)
//        -> image("../figures.png")
//        -> frame(80, 160, 100, 100)
//        -> mask(sf::Color(78, 144, 79).toInteger())
//        -> create();
//
//    gui -> sprite()
//            -> x(100)
//            -> y(100)
//            -> image("../figures.png")
//            -> frame(80, 160, 100, 100)
//            -> mask(sf::Color(78, 144, 79).toInteger())
//            -> create();

    while (gui->handleEvents()) {
        gui->display();
    }

    return 0;
}