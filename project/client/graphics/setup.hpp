#pragma once

#include "guiobj.hpp"

const int CELL_SIZE = 100;
const int CELL_TITLE_SIZE = 20;


void setupLogin(std::shared_ptr<GUIFactory> gui);

std::tuple<int, int> cell(std::string pos);

std::vector<GUIObj*> setupCells(std::shared_ptr<GUIFactory> gui);

void setupCellTitles(std::shared_ptr<GUIFactory> gui);

std::function<bool(sf::Event e)> onHover(GUIObj* obj);

std::function<bool(sf::Event e)> onClick(GUIObj* obj);

EventHandler figureEventHandler(GUISprite* figure, std::vector<GUIObj*> cells, std::shared_ptr<GUIFactory> gui);

void setupPawns(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setupRooks(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setupKnights(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setupBishops(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setupQueens(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setupKings(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setupFigures(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells);

void setup(std::shared_ptr<GUIFactory> gui);