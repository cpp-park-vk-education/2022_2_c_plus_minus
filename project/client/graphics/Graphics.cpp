#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUIObj.hpp"
#include "SetupBoard.hpp"
#include "SetupLogin.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;


int main() {
    sf::RenderWindow login(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Login");
    std::shared_ptr<GUIFactory> loginGui(new SFMLGUIFactory(&login));
    setupLogin(loginGui);
    while (loginGui->handleEvents()) {
        loginGui->display();
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess");
    std::shared_ptr<GUIFactory> gui(new SFMLGUIFactory(&window));
    setupBoard(gui);
    while (gui->handleEvents()) {
        gui->display();
    }

    return 0;
}