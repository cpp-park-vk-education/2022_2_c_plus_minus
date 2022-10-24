#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = WINDOW_WIDTH / 8;
const int CELL_TITLE_SIZE = 20;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

    std::array<std::array<sf::RectangleShape, 8>, 8> cells;
    std::array<sf::Text, 8> alphaCellTitle;
    std::array<sf::Text, 8> digitCellTitle;

    sf::Font cellTitleFont;
    if (!cellTitleFont.loadFromFile("../Montserrat-Regular.ttf")) {
        return 0;
    }

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto& cell = cells[row][col];
            int x = col * CELL_SIZE;
            int y = row * CELL_SIZE;
            cell.setSize({CELL_SIZE, CELL_SIZE});
            cell.setPosition({x, y});
            cell.setFillColor((row + col) % 2 ? sf::Color::Black : sf::Color::White);
        }
    }

    for (int i = 0; i < 8; ++i) {
        auto& title = alphaCellTitle[i];
        int row = 7;
        int col = i;
        auto& originCell = cells[row][col];
        title.setPosition(originCell.getPosition() + sf::Vector2f{CELL_SIZE - CELL_TITLE_SIZE, CELL_SIZE - CELL_TITLE_SIZE});
        title.setFont(cellTitleFont);
        title.setString(std::string{"abcdefgh"[i]});
        title.setCharacterSize(CELL_TITLE_SIZE);
        title.setFillColor(originCell.getFillColor() == sf::Color::Black ? sf::Color::White : sf::Color::Black);
    }

    for (int i = 0; i < 8; ++i) {
        auto& title = digitCellTitle[i];
        int row = i;
        int col = 0;
        auto& originCell = cells[row][col];
        title.setPosition(originCell.getPosition());
        title.setFont(cellTitleFont);
        title.setString(std::string{"12345678"[i]});
        title.setCharacterSize(CELL_TITLE_SIZE);
        title.setFillColor(originCell.getFillColor() == sf::Color::Black ? sf::Color::White : sf::Color::Black);
    }
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (auto& cellRows : cells) {
            for (auto& cell : cellRows) {
                window.draw(cell);
            }
        }

        for (auto& cellTitle : alphaCellTitle) {
            window.draw(cellTitle);
        }

        for (auto& cellTitle : digitCellTitle) {
            window.draw(cellTitle);
        }

        window.display();
    }

    return 0;
}