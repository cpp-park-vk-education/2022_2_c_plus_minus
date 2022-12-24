#include "GameUi.hpp"
#include "Client.hpp"

GameUi::GameUi() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chess"), gui(new SFMLGUIFactory(&window)){
//    window.close();
}

void GameUi::addClient(std::shared_ptr<Client> client) {

    client_ = client->weak_from_this();
}

std::string GameUi::coordsToStr(int x, int y){
    if (x > 800 || y > 800){
        return "nn";
    }
    std::string letters = "ABCDEFGH";
    std::string nums = "87654321";
    std::string result = "  ";
    result[0] = letters[x/100];
    result[1] = nums[y/100];
    return result;
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
    dynamic_cast<SFMLSprite*>(figPos[to]) -> x(x)
            -> y(y);
}

int GameUi::start() {
    std::queue<std::string> movesChan;
    setupBoard(gui, movesChan, figPos);
    std::string player1 = "First Player";
    std::string player2 = "Second Player";
    std::string room = "This room's name";
    setupInfo(gui, player1, player2, room);

    while (window.isOpen()){
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseMoved: {
//                    pos = window.mapPixelToCoords(Mouse::getPosition(window));
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    if (e.key.code == sf::Mouse::Left) {
                        std::cout << "pressed" << std::endl;
                        if (captured) {
                            pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                            finish_pos = coordsToStr(pos.x, pos.y);
                            if (finish_pos != "nn") {
                                captured = false;
                                auto client = client_.lock();
                                if (client)
                                    client->MoveFigure(start_pos + finish_pos);
                                continue;
                            }
                        }
                        pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        start_pos = coordsToStr(pos.x, pos.y);
                        if (start_pos != "nn") {
                            captured = true;
                        }
                        sf::Vector2i lp = sf::Mouse::getPosition(window);
                        std::cout << lp.x << ' ' << lp.y << std::endl;
                        break;
                    }
                }
                case sf::Event::MouseButtonReleased: {
                    if (e.key.code == sf::Mouse::Left) {
                        std::cout << "released" << std::endl;
                    }
                }
            }
        }
        gui->display();
    }
    return 0;
}