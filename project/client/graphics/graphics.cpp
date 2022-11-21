#include <SFML/Graphics.hpp>
#include "guiobj.hpp"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = WINDOW_WIDTH / 8;
const int CELL_TITLE_SIZE = 20;

int main() {
    GUIFactory* gui = new SFMLGUIFactory(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            gui.rect()
                -> x(col * CELL_SIZE)
                -> y(row * CELL_SIZE)
                -> width(CELL_SIZE)
                -> height(CELL_SIZE)
                -> color((row + col) % 2 ? 0 : -1)
                -> create();
        }
    }

    for (int i = 0; i < 8; ++i) {
        int row = 7;
        int col = i;
        gui.text()
            -> x(CELL_SIZE * (i + 1) - CELL_TITLE_SIZE)
            -> y(CELL_SIZE * 8 - CELL_TITLE_SIZE)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((row + col) % 2 ? -1 : 255)
            -> text(std::string{"abcdefgh"[i]})
            -> create();
    }

    for (int i = 0; i < 8; ++i) {
        int row = i;
        int col = 0;
        gui.text()
            -> x(0)
            -> y(CELL_SIZE * i)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((row + col) % 2 ? -1 : 255)
            -> text(std::string{"12345678"[i]})
            -> create();
    }

    while (gui.window.isOpen()) {
        sf::Event event;
        while (gui.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gui.window.close();
        }

        gui->window.clear();

        for (auto obj : gui.objects) {
            obj->draw();
        }

        gui->window.display();
    }

    return 0;
}