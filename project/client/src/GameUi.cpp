#include "GameUi.hpp"

#include "Client.hpp"

GameUi::GameUi()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess"),
      gui(new SFMLGUIFactory(&window)) {}

void GameUi::addClient(std::shared_ptr<Client> client) {
    client_ = client->weak_from_this();
}

void GameUi::finishGame() { is_finished = true; }

std::string GameUi::coordsToStr(int x, int y) {
    if (x > 800 || y > 800) {
        return "nn";
    }
    std::string letters = "ABCDEFGH";
    std::string nums = "87654321";
    std::string result = "  ";
    result[0] = letters[x / 100];
    result[1] = nums[y / 100];
    return result;
}

void GameUi::setupMsg(std::string msg) { setupMessage(gui, msg); }

void GameUi::setupRoomInfo(std::string player1, std::string player2,
                           std::string color, std::string room) {
    setupInfo(gui, player1, player2, color, room);
}

void GameUi::makeMove(std::string mov) {
    auto from = mov.substr(0, 2);
    auto to = mov.substr(2, 2);
    if (figPos.find(to) != figPos.end()) {
        gui->remove(figPos[to]);
    }
    figPos[to] = figPos[from];
    figPos.erase(from);
    auto [x, y] = cell(to);
    dynamic_cast<SFMLSprite*>(figPos[to])->x(x)->y(y);
}

void GameUi::reset() { gui.reset(); }

int GameUi::start() {
    std::queue<std::string> movesChan;
    setupBoard(gui, movesChan, figPos);
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    if (e.key.code == sf::Mouse::Left && !is_finished) {
                        if (captured) {
                            pos = window.mapPixelToCoords(
                                sf::Mouse::getPosition(window));
                            finish_pos = coordsToStr(pos.x, pos.y);
                            if (finish_pos != "nn") {
                                captured = false;
                                auto client = client_.lock();
                                if (client)
                                    client->MoveFigure(start_pos + finish_pos);
                                continue;
                            }
                        }
                        pos = window.mapPixelToCoords(
                            sf::Mouse::getPosition(window));
                        start_pos = coordsToStr(pos.x, pos.y);
                        if (start_pos != "nn") {
                            captured = true;
                        }
                        break;
                    }
                }
                case sf::Event::MouseButtonReleased: {
                    if (e.key.code == sf::Mouse::Left && !is_finished) {
                    }
                }
            }
        }
        gui->display();
    }
    return 0;
}