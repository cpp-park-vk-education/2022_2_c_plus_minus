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

std::vector<GUIObj*> setupCells(std::shared_ptr<GUIFactory> gui, figure_color color) {
    std::vector<GUIObj*> cells;
    std::string letters;
    std::string numbers;

    letters = "ABCDEFGH";
    numbers = "12345678";
    if (color == BLACK){
        letters = "HGFEDCBA";
        numbers = "87654321";
    }
    if (color == figure_color::WHITE) {
        for (auto letter: letters) {
            for (auto number: numbers) {
                auto [x, y] = cell(std::string{letter, number});
                auto cell =
                        gui->rect()
                                ->x(x)
                                ->y(y)
                                ->width(CELL_SIZE)
                                ->height(CELL_SIZE)
                                ->color((letter - 'A' + number - '1') % 2 ? 0xf0fff0ffu
                                                                          : 0x01796fff);
                cells.push_back(cell);
                cell->create();
            }
        }
    } else  {
        for (auto letter: letters) {
            for (auto number: numbers) {
                auto [x, y] = cell(std::string{letter, number});
                auto cell =
                        gui->rect()
                                ->x(x)
                                ->y(y)
                                ->width (CELL_SIZE)
                                ->height(CELL_SIZE)
                                ->color((letter - 'A' + number - '1') % 2 ? 0xf0fff0ffu
                                                                          : 0x01796fff);
                cells.push_back(cell);
                cell->create();
            }
        }
    }
    return cells;
}

void setupCellTitles(std::shared_ptr<GUIFactory> gui) {
    for (auto letter : "ABCDEFGH") {
        char number = '1';
        auto [x, y] = cell(std::string{letter, number});
        gui->text()
                ->x(x + CELL_SIZE - CELL_TITLE_SIZE)
                ->y(y + CELL_SIZE - CELL_TITLE_SIZE)
                ->font("Montserrat-Regular.ttf")
                ->size(CELL_TITLE_SIZE)
                ->color((letter - 'A' + number - '1') % 2
                        ? sf::Color::Black.toInteger()
                        : sf::Color::White.toInteger())
                ->text(std::string{letter})
                ->create();
    }
    for (auto number : "12345678") {
        char letter = 'A';
        auto [x, y] = cell(std::string{letter, number});
        gui->text()
                ->x(x)
                ->y(y)
                ->font("Montserrat-Regular.ttf")
                ->size(CELL_TITLE_SIZE)
                ->color((letter - 'A' + number - '1') % 2
                        ? sf::Color::Black.toInteger()
                        : sf::Color::White.toInteger())
                ->text(std::string{number})
                ->create();
    }
}

void setupBlackCellTitles(std::shared_ptr<GUIFactory> gui) {
    int counter = 0;
    std::string letters  = "HGFEDCBA";
    for (auto letter : letters) {
        char number = '1';
        auto [x, y] = cell(std::string{letter, number});
        gui->text()
            ->x(x + CELL_SIZE - CELL_TITLE_SIZE)
            ->y(y + CELL_SIZE - CELL_TITLE_SIZE)
            ->font("Montserrat-Regular.ttf")
            ->size(CELL_TITLE_SIZE)
            ->color((letter - 'A' + number - '1') % 2
                        ? sf::Color::Black.toInteger()
                        : sf::Color::White.toInteger())
            ->text(std::string{letters[7-counter]})
            ->create();
        counter++;
    }
    counter = 0;
    std::string nums  = "87654321";
    for (auto number : nums) {
        char letter = 'A';
        auto [x, y] = cell(std::string{letter, number});
        gui->text()
            ->x(x)
            ->y(y)
            ->font("Montserrat-Regular.ttf")
            ->size(CELL_TITLE_SIZE)
            ->color((letter - 'A' + number - '1') % 2
                    ? sf::Color::Black.toInteger()
                    : sf::Color::White.toInteger())
            ->text(std::string{nums[7-counter]})
            ->create();
        counter++;
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

void setupPawns(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color) {


    char number;
    if (color == figure_color::WHITE){
        number = '7';
    } else number ='2';
    for (auto letter : "ABCDEFGH") {
        auto pos = std::string{letter, number};
        auto [x, y] = cell(pos);
        auto pawn = gui->sprite()
                        ->x(x)
                        ->y(y)
                        ->image("figures.png")
                        ->frame(1000, 200, 200, 200)
                        ->scale(CELL_SIZE / 200.0);
        figPos[pos] = pawn;
        pawn->create();
    }
    if (color == figure_color::WHITE){
        number = '2';
    } else number ='7';

    for (auto letter : "ABCDEFGH") {
        auto pos = std::string{letter, number};
        auto [x, y] = cell(pos);
        auto pawn = gui->sprite()
                        ->x(x)
                        ->y(y)
                        ->image("figures.png")
                        ->frame(1000, 0, 200, 200)
                        ->scale(CELL_SIZE / 200.0);
        figPos[pos] = pawn;
        pawn->create();
    }
}

void setupRooks(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color) {
    using namespace std::literals;
    if (color == figure_color::WHITE) {
        for (auto pos: {"A1"s, "H1"s}) {
            auto [x, y] = cell(pos);
            auto rook = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(800, 0, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = rook;
            rook->create();
        }
        for (auto pos: {"A8"s, "H8"s}) {
            auto [x, y] = cell(pos);
            auto rook = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(800, 200, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = rook;
            rook->create();
        }
    } else {
        for (auto pos: {"A8"s, "H8"s}) {
            auto [x, y] = cell(pos);
            auto rook = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(800, 0, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = rook;
            rook->create();
        }
        for (auto pos: {"A1"s, "H1"s}) {
            auto [x, y] = cell(pos);
            auto rook = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(800, 200, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = rook;
            rook->create();
        }
    }
}

void setupKnights(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                  std::queue<std::string>& movesChan,
                  std::map<std::string, GUIObj*>& figPos, figure_color color) {
    using namespace std::literals;
    if (color == figure_color::WHITE) {
        for (auto pos: {"B1"s, "G1"s}) {
            auto [x, y] = cell(pos);
            auto knight = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(600, 0, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = knight;
            knight->create();
        }
        for (auto pos: {"B8"s, "G8"s}) {
            auto [x, y] = cell(pos);
            auto knight = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(600, 200, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = knight;
            knight->create();
        }
    } else {
        for (auto pos: {"B8"s, "G8"s}) {
            auto [x, y] = cell(pos);
            auto knight = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(600, 0, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = knight;
            knight->create();
        }
        for (auto pos: {"B1"s, "G1"s}) {
            auto [x, y] = cell(pos);
            auto knight = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(600, 200, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = knight;
            knight->create();
        }
    }
}

void setupBishops(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                  std::queue<std::string>& movesChan,
                  std::map<std::string, GUIObj*>& figPos, figure_color color) {
    using namespace std::literals;
    if (color == figure_color::WHITE) {
        for (auto pos: {"C1"s, "F1"s}) {
            auto [x, y] = cell(pos);
            auto bishop = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(400, 0, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = bishop;
            bishop->create();
        }
        for (auto pos: {"C8"s, "F8"s}) {
            auto [x, y] = cell(pos);
            auto bishop = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(400, 200, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = bishop;
            bishop->create();
        }
    } else {
        for (auto pos: {"C8"s, "F8"s}) {
            auto [x, y] = cell(pos);
            auto bishop = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(400, 0, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = bishop;
            bishop->create();
        }
        for (auto pos: {"C1"s, "F1"s}) {
            auto [x, y] = cell(pos);
            auto bishop = gui->sprite()
                    ->x(x)
                    ->y(y)
                    ->image("figures.png")
                    ->frame(400, 200, 200, 200)
                    ->scale(CELL_SIZE / 200.0);

            figPos[pos] = bishop;
            bishop->create();
        }
    }

}

void setupQueens(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                 std::queue<std::string>& movesChan,
                 std::map<std::string, GUIObj*>& figPos, figure_color color) {
    using namespace std::literals;
    if (color == figure_color::WHITE) {
        auto [x, y] = cell("D1"s);
        auto queen1 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(200, 0, 200, 200)
                ->scale(CELL_SIZE / 200.0);

        figPos["D1"] = queen1;
        queen1->create();
        std::tie(x, y) = cell("D8"s);
        auto queen2 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(200, 200, 200, 200)
                ->scale(CELL_SIZE / 200.0);

        figPos["D8"] = queen2;
        queen2->create();
    } else {
        auto [x, y] = cell("E8"s);
        auto queen1 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(200, 0, 200, 200)
                ->scale(CELL_SIZE / 200.0);

        figPos["E8"] = queen1;
        queen1->create();
        std::tie(x, y) = cell("E1"s);
        auto queen2 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(200, 200, 200, 200)
                ->scale(CELL_SIZE / 200.0);

        figPos["E1"] = queen2;
        queen2->create();
    }
}

void setupKings(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color) {
    using namespace std::literals;
    if (color == figure_color::WHITE) {
        auto [x, y] = cell("E1"s);
        auto king1 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(0, 0, 200, 200)
                ->scale(CELL_SIZE / 200.0);
        figPos["E1"] = king1;
        king1->create();
        std::tie(x, y) = cell("E8"s);
        auto king2 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(0, 200, 200, 200)
                ->scale(CELL_SIZE / 200.0);
        figPos["E8"] = king2;
        king2->create();
    } else {
        auto [x, y] = cell("D8"s);
        auto king1 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(0, 0, 200, 200)
                ->scale(CELL_SIZE / 200.0);
        figPos["D8"] = king1;
        king1->create();
        std::tie(x, y) = cell("D1"s);
        auto king2 = gui->sprite()
                ->x(x)
                ->y(y)
                ->image("figures.png")
                ->frame(0, 200, 200, 200)
                ->scale(CELL_SIZE / 200.0);
        figPos["D1"] = king2;
        king2->create();
    }
}

void setupFigures(std::shared_ptr<GUIFactory> gui, std::vector<GUIObj*> cells,
                  std::queue<std::string>& movesChan,
                  std::map<std::string, GUIObj*>& figPos, figure_color color) {
    setupPawns(gui, cells, movesChan, figPos, color);
    setupRooks(gui, cells, movesChan, figPos, color);
    setupKnights(gui, cells, movesChan, figPos, color);
    setupBishops(gui, cells, movesChan, figPos, color);
    setupQueens(gui, cells, movesChan, figPos, color);
    setupKings(gui, cells, movesChan, figPos, color);
}

void setupBoard(std::shared_ptr<GUIFactory> gui,
                std::queue<std::string>& movesChan,
                std::map<std::string, GUIObj*>& figPos, figure_color color) {
    auto cells = setupCells(gui, color);
    if (color == figure_color::WHITE) {
        setupCellTitles(gui);
    } else {
        setupBlackCellTitles(gui);
    }
    setupFigures(gui, cells, movesChan, figPos, color);
}

void setupMessage(std::shared_ptr<GUIFactory> gui, std::string message, uint32_t color) {
    gui->remove(gui->current_msg);
    auto text_obj = gui->text();
    text_obj->x(810)
        ->y(50)
        ->size(30)
        ->color(color)
        ->text(message)
        ->font("Montserrat-Regular.ttf")
        ->create();
    gui->current_msg = text_obj;
}

void setupInfo(std::shared_ptr<GUIFactory> gui, std::string player1,
               std::string player2, std::string color, std::string room) {
    std::replace(player1.begin(), player1.end(), ' ', '\n');
    std::replace(player2.begin(), player2.end(), ' ', '\n');
    std::replace(room.begin(), room.end(), ' ', '\n');
    room = "Room:\n" + room;
    color = "Your color:\n" + color;
    player1 = "You:\n" + player1;
    player2 = "Enemy:\n" + player2;
    gui->rect()
        ->x(800)
        ->y(0)
        ->width(400)
        ->height(800)
        ->color(0x84c3beff)
        ->create();

    gui->text()
        ->x(810)
        ->y(150)
        ->size(30)
        ->color(0xffffffffu)
        ->text(player1)
        ->font("Montserrat-Regular.ttf")
        ->create();

    gui->text()
        ->x(810)
        ->y(250)
        ->size(30)
        ->color(0xffffffffu)
        ->text(color)
        ->font("Montserrat-Regular.ttf")
        ->create();

    gui->text()
        ->x(810)
        ->y(450)
        ->size(30)
        ->color(0xffffffffu)
        ->text(room)
        ->font("Montserrat-Regular.ttf")
        ->create();

    gui->text()
        ->x(810)
        ->y(700)
        ->size(30)
        ->color(0xffffffffu)
        ->text(player2)
        ->font("Montserrat-Regular.ttf")
        ->create();
}
