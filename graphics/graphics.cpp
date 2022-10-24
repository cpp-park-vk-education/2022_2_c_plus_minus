#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 200;
const int WINDOW_HEIGHT = 200;
const int CELL_SIDE = WINDOW_WIDTH / 8;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

    std::array<sf::RectangleShape, 64> cells;
    for (int i = 0; i < 64; ++i) {
        auto& cell = cells[i];
        int y = i / 8;
        int x = i % 8;
        cell.setSize({CELL_SIDE, CELL_SIDE});
        cell.setPosition({x * CELL_SIDE, y * CELL_SIDE});
        cell.setFillColor((x + y) % 2 ? sf::Color::Black : sf::Color::White);
    }
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (auto& cell : cells) {
            window.draw(cell);
        }

        window.display();
    }

    return 0;
}