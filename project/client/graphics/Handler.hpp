#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUIObj.hpp"
#include "SetupBoard.hpp"

#define DEBUG

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

using namespace sf;

class GameHandler{
public:
    GameHandler() : gui(new SFMLGUIFactory(&window)), window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess"){

        std::queue<std::string> movesChan;
        setupBoard(gui, movesChan, figPos);

        std::string player1 = "First Player";
        std::string player2 = "Second Player";
        std::string room = "This room's name";
        setupInfo(gui, player1, player2, room);
    }
    void makeMove(std::string mov);
    int start();

private:
    std::shared_ptr<GUIFactory> gui;
    std::map<std::string, GUIObj*> figPos;
    sf::RenderWindow window;
    Vector2f pos;
    bool captured = false;
    std::string start_pos = "  ";
    std::string finish_pos = "  ";
};



void GameHandler::makeMove(std::string mov){

}

std::string coordsToStr(int x, int y);

int GameHandler::start() {
    while (window.isOpen()){
        Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
                case Event::Closed: {
                    window.close();
                    break;
                }
                case Event::MouseMoved: {
                    pos = window.mapPixelToCoords(Mouse::getPosition(window));
                    break;
                }
                case Event::MouseButtonPressed: {
                    if (e.key.code == Mouse::Left) {
                        std::cout << "pressed" << std::endl;
                        if (captured) {
                            pos = window.mapPixelToCoords(Mouse::getPosition(window));
                            finish_pos = coordsToStr(pos.x, pos.y);
                            if (finish_pos != "nn") {
                                captured = false;
                                makeMove(start_pos + finish_pos);
                                continue;
                            }
                        }
                        pos = window.mapPixelToCoords(Mouse::getPosition(window));
                        start_pos = coordsToStr(pos.x, pos.y);
                        if (start_pos != "nn") {
                            captured = true;
                        }
                        sf::Vector2i lp = sf::Mouse::getPosition(window);
                        std::cout << lp.x << ' ' << lp.y << std::endl;
                        break;
                    }
                }
                case Event::MouseButtonReleased: {
                    if (e.key.code == Mouse::Left) {
                        std::cout << "released" << std::endl;
                    }
                }
            }
        }
        gui->display();
     }
    return 0;
}