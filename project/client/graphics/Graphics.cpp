#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUIObj.hpp"
#include "SetupBoard.hpp"
#include "SetupLogin.hpp"

const int WINDOW_WIDTH = 1000;
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

    std::queue<std::string> movesChan;
    setupBoard(gui, movesChan);

    std::string player1 = "First Player";
    std::string player2 = "Second Player";
    std::string room = "This room's name";
    setupInfo(gui, player1, player2, room);

    while (gui->handleEvents()) {
        while (!movesChan.empty()) {
            std::cerr << movesChan.front() << '\n';
            movesChan.pop();
        }
        gui->display();
    }

    return 0;
}