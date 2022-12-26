#pragma once

#include <queue>
#include "Base.hpp"
#include "GUIObj.hpp"

const int CELL_SIZE = 100;
const int CELL_TITLE_SIZE = 20;

std::string pos(int x, int y);

std::tuple<int, int> cell(std::string pos);

std::vector<GUIObj*> setupBlackCells(std::shared_ptr<GUIFactory> gui);
std::vector<GUIObj*> setupCells(std::shared_ptr<GUIFactory> gui,  figure_color color);

void setupCellTitles(std::shared_ptr<GUIFactory> gui);
void setupBlackCellTitles(std::shared_ptr<GUIFactory> gui);

std::function<bool(sf::Event e)> onHover(GUIObj* obj);

std::function<bool(sf::Event e)> onClick(GUIObj* obj);

EventHandler figureEventHandler(GUISprite* figure, std::vector<GUIObj*> cells,
                                std::shared_ptr<GUIFactory> gui,
                                std::queue<std::string>& movesChan,
                                std::map<std::string, GUIObj*>& figPos);

void setupPawns(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupRooks(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupKnights(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                  std::queue<std::string>& movesChan,
                  std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupBishops(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                  std::queue<std::string>& movesChan,
                  std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupQueens(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                 std::queue<std::string>& movesChan,
                 std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupKings(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupFigures(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                  std::queue<std::string>& movesChan,
                  std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupBoard(std::shared_ptr<GUIFactory> gui,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color);

void setupInfo(std::shared_ptr<GUIFactory> gui, std::string player1,
               std::string player2, std::string color, std::string room);

void setupMessage(std::shared_ptr<GUIFactory> gui, std::string message, uint32_t color = sf::Color::Black.toInteger());