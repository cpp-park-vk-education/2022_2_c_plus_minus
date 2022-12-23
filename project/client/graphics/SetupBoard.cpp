#include "SetupBoard.hpp"


std::tuple<int, int> cell(std::string pos) {
    char letter = pos[0];
    char number = pos[1];
    int x = letter - 'A';
    int y = number - '1';
    return std::tuple{CELL_SIZE * x, CELL_SIZE * (7 - y)};
}

std::string pos(int x, int y) {
    return std::string{"ABCDEFGH"[x / CELL_SIZE], "87654321"[y / CELL_SIZE]};
}

std::vector<GUIObj*> setupCells(std::shared_ptr<GUIFactory> gui) {
    std::vector<GUIObj*> cells;
    for (auto letter : "ABCDEFGH") {
        for (auto number : "12345678") {
            auto [x, y] = cell(std::string{letter, number});
            auto cell = gui -> rect()
                            -> x(x)
                            -> y(y)
                            -> width(CELL_SIZE)
                            -> height(CELL_SIZE)
                            -> color((letter - 'A' + number - '1') % 2 ? 0x01796fffu : 0xf0fff0ffu);
            cells.push_back(cell);
            cell -> create();
        }
    }
    return cells;
}

void setupCellTitles(std::shared_ptr<GUIFactory> gui) {
    for (auto letter : "ABCDEFGH") {
        char number = '1';
        auto [x, y] = cell(std::string{letter, number});
        gui -> text()
            -> x(x + CELL_SIZE - CELL_TITLE_SIZE)
            -> y(y + CELL_SIZE - CELL_TITLE_SIZE)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((letter - 'A' + number - '1') % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{letter})
            -> create();
    }
    for (auto number : "12345678") {
        char letter = 'A';
        auto [x, y] = cell(std::string{letter, number});
        gui -> text()
            -> x(x)
            -> y(y)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((letter - 'A' + number - '1') % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{number})
            -> create();
    }
}

std::function<bool(sf::Event e)> onHover(GUIObj* obj) {
    return [obj](sf::Event e) -> bool {
        if (e.type == sf::Event::MouseMoved) {
            auto [x, y] = e.mouseMove;
            if (obj->contains(x, y)) {
                return true;
            }
        }
        return false;
    };
}

std::function<bool(sf::Event e)> onClick(GUIObj* obj) {
    return [obj](sf::Event e) -> bool {
        if (e.type == sf::Event::MouseButtonPressed) {
            auto [button, x, y] = e.mouseButton;
            if (obj->contains(x, y) && button == sf::Mouse::Left) {
                return true;
            }
        }
        return false;
    };
}
#include <iostream>
//EventHandler figureEventHandler(GUISprite* figure, std::vector<GUIObj*> cells, std::shared_ptr<GUIFactory> gui, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
//    return EventHandler(onClick(figure), [figure, cells, gui, &movesChan, &figPos]() mutable -> size_t {
//        for (auto cell : cells) {
//            EventHandler eh(onClick(cell), [figure, cell, gui, &movesChan, &figPos]() mutable -> size_t {
//                if (figure->getX() == cell->getX() && figure->getY() == cell->getY()) {
//                    return 64 + 1;
//                }
//                auto move = pos(figure->getX(), figure->getY()) + pos(cell->getX(), cell->getY());
//                movesChan.push(move);
//                if (auto cellPos = pos(cell->getX(), cell->getY()); figPos.contains(cellPos)) {
//                    gui->remove(figPos[cellPos]);
//                }
//                figPos.erase(pos(figure->getX(), figure->getY()));
//                figure -> x(cell->getX())
//                       -> y(cell->getY());
//                figPos[pos(figure->getX(), figure->getY())] = figure;
//                return 64;
//            });
//            gui->addEventHandler(eh);
//        }
//        return 0;
//    });
//}

void setupPawns(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    char number = '7';
    for (auto letter : "ABCDEFGH") {
        auto pos = std::string{letter, number};
        auto [x, y] = cell(pos);
        auto pawn = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(1000, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(pawn, cells, gui, movesChan, figPos));
        figPos[pos] = pawn;
        pawn -> create();
    }

    number = '2';
    for (auto letter : "ABCDEFGH") {
        auto pos = std::string{letter, number};
        auto [x, y] = cell(pos);
        auto pawn = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(1000, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(pawn, cells, gui, movesChan, figPos));
        figPos[pos] = pawn;
        pawn->create();
    }
}

void setupRooks(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    using namespace std::literals;
    for (auto pos : {"A1"s, "H1"s}) {
        auto [x, y] = cell(pos);
        auto rook = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(800, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(rook, cells, gui, movesChan, figPos));
        figPos[pos] = rook;
        rook->create();
    }
    for (auto pos : {"A8"s, "H8"s}) {
        auto [x, y] = cell(pos);
        auto rook = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(800, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(rook, cells, gui, movesChan, figPos));
        figPos[pos] = rook;
        rook->create();
    }
}

void setupKnights(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    using namespace std::literals;
    for (auto pos : {"B1"s, "G1"s}) {
        auto [x, y] = cell(pos);
        auto knight = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(600, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(knight, cells, gui, movesChan, figPos));
        figPos[pos] = knight;
        knight->create();
    }
    for (auto pos : {"B8"s, "G8"s}) {
        auto [x, y] = cell(pos);
        auto knight = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(600, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(knight, cells, gui, movesChan, figPos));
        figPos[pos] = knight;
        knight->create();
    }
}

void setupBishops(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    using namespace std::literals;
    for (auto pos : {"C1"s, "F1"s}) {
        auto [x, y] = cell(pos);
        auto bishop = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(400, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(bishop, cells, gui, movesChan, figPos));
        figPos[pos] = bishop;
        bishop->create();
    }
    for (auto pos : {"C8"s, "F8"s}) {
        auto [x, y] = cell(pos);
        auto bishop = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("figures.png")
                        -> frame(400, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
//        gui->addEventHandler(figureEventHandler(bishop, cells, gui, movesChan, figPos));
        figPos[pos] = bishop;
        bishop->create();
    }
}

void setupQueens(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    using namespace std::literals;
    auto [x, y] = cell("D1"s);
    auto queen1 = gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("figures.png")
        -> frame(200, 0, 200, 200)
        -> scale(CELL_SIZE / 200.0);
//    gui->addEventHandler(figureEventHandler(queen1, cells, gui, movesChan, figPos));
    figPos["D1"] = queen1;
    queen1->create();
    std::tie(x, y) = cell("D8"s);
    auto queen2 = gui -> sprite()
            -> x(x)
            -> y(y)
            -> image("figures.png")
            -> frame(200, 200, 200, 200)
            -> scale(CELL_SIZE / 200.0);
//    gui->addEventHandler(figureEventHandler(queen2, cells, gui, movesChan, figPos));
    figPos["D8"] = queen2;
    queen2->create();
}

void setupKings(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    using namespace std::literals;
    auto [x, y] = cell("E1"s);
    auto king1 = gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("figures.png")
        -> frame(0, 0, 200, 200)
        -> scale(CELL_SIZE / 200.0);
//    gui->addEventHandler(figureEventHandler(king1, cells, gui, movesChan, figPos));
    figPos["E1"] = king1;
    king1 -> create();
    std::tie(x, y) = cell("E8"s);
    auto king2 = gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("figures.png")
        -> frame(0, 200, 200, 200)
        -> scale(CELL_SIZE / 200.0);
//    gui->addEventHandler(figureEventHandler(king2, cells, gui, movesChan, figPos));
    figPos["E8"] = king2;
    king2 -> create();
}

void setupFigures(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    setupPawns(gui, cells, movesChan, figPos);
    setupRooks(gui, cells, movesChan, figPos);
    setupKnights(gui, cells, movesChan, figPos);
    setupBishops(gui, cells, movesChan, figPos);
    setupQueens(gui, cells, movesChan, figPos);
    setupKings(gui, cells, movesChan, figPos);
}

void setupBoard(std::shared_ptr<GUIFactory> gui, std::queue<std::string>& movesChan, std::map<std::string, GUIObj*>& figPos) {
    auto cells = setupCells(gui);
    setupCellTitles(gui);
    setupFigures(gui, cells, movesChan, figPos);
}

void setupInfo(std::shared_ptr<GUIFactory> gui, std::string player1, std::string player2, std::string room) {
    std::replace(player1.begin(), player1.end(), ' ', '\n');
    std::replace(player2.begin(), player2.end(), ' ', '\n');
    std::replace(room.begin(), room.end(), ' ', '\n');
    room = "Room:\n\n" + room;
    gui -> rect()
        -> x(800)
        -> y(0)
        -> width(200)
        -> height(800)
        -> color(0x84c3beff)
        ->create();

    gui -> text()
        -> x(810)
        -> y(50)
        -> size(30)
        -> color(0xffffffffu)
        -> text(player1)
        -> font("Montserrat-Regular.ttf")
        -> create();

    gui -> text()
        -> x(810)
        -> y(700)
        -> size(30)
        -> color(0xffffffffu)
        -> text(player2)
        -> font("Montserrat-Regular.ttf")
        -> create();

    gui -> text()
        -> x(810)
        -> y(300)
        -> size(30)
        -> color(0xffffffffu)
        -> text(room)
        -> font("Montserrat-Regular.ttf")
        -> create();
}
