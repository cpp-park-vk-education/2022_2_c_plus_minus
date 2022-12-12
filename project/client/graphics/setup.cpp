#include "guiobj.hpp"

const int CELL_SIZE = 100;
const int CELL_TITLE_SIZE = 20;


std::tuple<int, int> cell(std::string pos) {
    char letter = pos[0];
    char number = pos[1];
    int x = letter - 'a';
    int y = number - '1';
    return std::tuple{CELL_SIZE * x, CELL_SIZE * (7 - y)};
}

void setupCells(std::shared_ptr<GUIFactory> gui) {
    for (auto letter : "abcdefgh") {
        for (auto number : "12345678") {
            auto [x, y] = cell(std::string{letter, number});
            gui -> rect()
                -> x(x)
                -> y(y)
                -> width(CELL_SIZE)
                -> height(CELL_SIZE)
                -> color((letter - 'a' + number - '1') % 2 ? 0x01796fffu : 0xf0fff0ffu)
                -> create();
        }
    }
}

void setupCellTitles(std::shared_ptr<GUIFactory> gui) {
    for (auto letter : "abcdefgh") {
        char number = '1';
        auto [x, y] = cell(std::string{letter, number});
        gui -> text()
            -> x(x + CELL_SIZE - CELL_TITLE_SIZE)
            -> y(y + CELL_SIZE - CELL_TITLE_SIZE)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((letter - 'a' + number - '1') % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
            -> text(std::string{letter})
            -> create();
    }
    for (auto number : "12345678") {
        char letter = 'a';
        auto [x, y] = cell(std::string{letter, number});
        gui -> text()
            -> x(x)
            -> y(y)
            -> font("Montserrat-Regular.ttf")
            -> size(CELL_TITLE_SIZE)
            -> color((letter - 'a' + number - '1') % 2 ? sf::Color::White.toInteger() : sf::Color::Black.toInteger())
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

void setupPawns(std::shared_ptr<GUIFactory> gui) {
    char number = '7';
    for (auto letter : "abcdefgh") {
        auto [x, y] = cell(std::string{letter, number});
        auto pawn = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(1000, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(pawn),
                [letter, number]() -> void {
                    std::cerr << "pawn at " << letter << number << '\n';
                });
        gui->addEventHandler(eh);
        pawn -> create();
    }

    number = '2';
    for (auto letter : "abcdefgh") {
        auto [x, y] = cell(std::string{letter, number});
        auto pawn = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(1000, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(pawn),
                [letter, number]() -> void {
                    std::cerr << "pawn at " << letter << number << '\n';
                });
        gui->addEventHandler(eh);
        pawn->create();
    }
}

void setupRooks(std::shared_ptr<GUIFactory> gui) {
    using namespace std::literals;
    for (auto pos : {"a1"s, "h1"s}) {
        auto [x, y] = cell(pos);
        auto rook = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(800, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(rook),
                [pos]() -> void {
                    std::cerr << "rook at " << pos << '\n';
                });
        gui->addEventHandler(eh);
        rook->create();
    }
    for (auto pos : {"a8"s, "h8"s}) {
        auto [x, y] = cell(pos);
        auto rook = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(800, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(rook),
                [pos]() -> void {
                    std::cerr << "rook at " << pos << '\n';
                });
        gui->addEventHandler(eh);
        rook->create();
    }
}

void setupKnights(std::shared_ptr<GUIFactory> gui) {
    using namespace std::literals;
    for (auto pos : {"b1"s, "g1"s}) {
        auto [x, y] = cell(pos);
        auto knight = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(600, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(knight),
                [pos]() -> void {
                    std::cerr << "knight at " << pos << '\n';
                });
        gui->addEventHandler(eh);
        knight->create();
    }
    for (auto pos : {"b8"s, "g8"s}) {
        auto [x, y] = cell(pos);
        auto knight = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(600, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(knight),
                [pos]() -> void {
                    std::cerr << "knight at " << pos << '\n';
                });
        gui->addEventHandler(eh);
        knight->create();
    }
}

void setupBishops(std::shared_ptr<GUIFactory> gui) {
    using namespace std::literals;
    for (auto pos : {"c1"s, "f1"s}) {
        auto [x, y] = cell(pos);
        auto bishop = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(400, 0, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(bishop),
                [pos]() -> void {
                    std::cerr << "bishop at " << pos << '\n';
                });
        gui->addEventHandler(eh);
        bishop->create();
    }
    for (auto pos : {"c8"s, "f8"s}) {
        auto [x, y] = cell(pos);
        auto bishop = gui -> sprite()
                        -> x(x)
                        -> y(y)
                        -> image("../figures.png")
                        -> frame(400, 200, 200, 200)
                        -> scale(CELL_SIZE / 200.0);
        EventHandler eh(onHover(bishop),
                [pos]() -> void {
                    std::cerr << "bishop at " << pos << '\n';
                });
        gui->addEventHandler(eh);
        bishop->create();
    }
}

void setupQueens(std::shared_ptr<GUIFactory> gui) {
    using namespace std::literals;
    auto [x, y] = cell("d1"s);
    gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("../figures.png")
        -> frame(200, 0, 200, 200)
        -> scale(CELL_SIZE / 200.0)
        -> create();
    std::tie(x, y) = cell("d8"s);
    gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("../figures.png")
        -> frame(200, 200, 200, 200)
        -> scale(CELL_SIZE / 200.0)
        -> create();
}

void setupKings(std::shared_ptr<GUIFactory> gui) {
    using namespace std::literals;
    auto [x, y] = cell("e1"s);
    gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("../figures.png")
        -> frame(0, 0, 200, 200)
        -> scale(CELL_SIZE / 200.0)
        -> create();
    std::tie(x, y) = cell("e8"s);
    gui -> sprite()
        -> x(x)
        -> y(y)
        -> image("../figures.png")
        -> frame(0, 200, 200, 200)
        -> scale(CELL_SIZE / 200.0)
        -> create();
}

void setupFigures(std::shared_ptr<GUIFactory> gui) {
    setupPawns(gui);
    setupRooks(gui);
    setupKnights(gui);
    setupBishops(gui);
    setupQueens(gui);
    setupKings(gui);
}

void setup(std::shared_ptr<GUIFactory> gui) {
    setupCells(gui);
    setupCellTitles(gui);
    setupFigures(gui);
}