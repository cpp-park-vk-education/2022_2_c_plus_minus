#pragma once

#include "GUIObj.hpp"
#include <queue>

const int CELL_SIZE = 100;
const int CELL_TITLE_SIZE = 20;


std::string pos(int x, int y);

std::tuple<int, int> cell(std::string pos);

std::vector<GUIObj*> setupCells(std::shared_ptr<GUIFactory> gui);

void setupCellTitles(std::shared_ptr<GUIFactory> gui);

std::function<bool(sf::Event e)> onHover(GUIObj* obj);

std::function<bool(sf::Event e)> onClick(GUIObj* obj);

EventHandler figureEventHandler(GUISprite* figure, std::vector<GUIObj*> cells, std::shared_ptr<GUIFactory> gui, std::queue<std::string>& movesChan);

void setupPawns(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupRooks(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupKnights(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupBishops(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupQueens(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupKings(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupFigures(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan);

void setupBoard(std::shared_ptr<GUIFactory> gui, std::queue<std::string>& movesChan);

void setupInfo(std::shared_ptr<GUIFactory> gui, std::string player1, std::string player2, std::string room);