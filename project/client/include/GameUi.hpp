#pragma once

#include    <SFML/Graphics.hpp>
#include <iostream>
#include "GUIObj.hpp"
#include "SetupBoard.hpp"
//#include "Client.hpp"
#define DEBUG

class Client;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

class GameUi : public std::enable_shared_from_this<GameUi>{
public:
    GameUi();
    void makeMove(std::string mov);
    int start();
    void addClient(std::shared_ptr<Client> client);
    std::string coordsToStr(int x, int y);
//    setupInfo(gui, player1, player2, room);

private:
    sf::RenderWindow window;
    std::shared_ptr<GUIFactory> gui;
    std::map<std::string, GUIObj*> figPos;
    sf::Vector2f pos;
    bool captured = false;
    std::string start_pos = "  ";
    std::string finish_pos = "  ";
    std::weak_ptr<Client> client_;
};

