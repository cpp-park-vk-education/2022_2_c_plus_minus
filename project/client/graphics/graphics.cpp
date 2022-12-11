#include <SFML/Graphics.hpp>
#include <iostream>
#include "guiobj.hpp"
#include "setup.cpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;


int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "chess");
    std::shared_ptr<GUIFactory> gui(new SFMLGUIFactory(&window));

    setup(gui);

    while (gui->handleEvents()) {
        gui->display();
    }

    return 0;
}