#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GUIObj.hpp"
#include "SetupBoard.hpp"

#define DEBUG

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

using namespace sf;

class GameUi{
public:
    GameUi() ;
    void makeMove(std::string mov);
    int start();
    std::string coordsToStr(int x, int y);
//    setupInfo(gui, player1, player2, room);

private:
    sf::RenderWindow window;
    std::shared_ptr<GUIFactory> gui;
    std::map<std::string, GUIObj*> figPos;
    Vector2f pos;
    bool captured = false;
    std::string start_pos = "  ";
    std::string finish_pos = "  ";
};

