#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <string>



const int width = 20;
const int height = 20;
const int cellSize = 20;

int main() {
    sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Snake Game");
    window.setFramerateLimit(144);

    srand(static_cast<unsigned>(time(nullptr)));
	int score = 0;
    sf::Vector2i direction(1, 0);

    std::vector<sf::Vector2i> snake = { {width / 2, height / 2} };
    sf::Vector2i newFood;
    do {
        newFood.x = rand() % width;
        newFood.y = rand() % height;
    } while (std::find(snake.begin(), snake.end(), newFood) != snake.end());

    int foodX = newFood.x;
	int foodY = newFood.y;

    sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));

    sf::Clock clock;
    float moveDelay = 0.1f;

	bool gameOver = false;

	sf::Font font;
	if (!font.loadFromFile("LiberationSans.ttf")) {
		return -1; // Error loading font
	}
    sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(5, 0);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (!gameOver && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && direction.y != 1) direction = { 0, -1 };
                else if (event.key.code == sf::Keyboard::Down && direction.y != -1) direction = { 0, 1 };
                else if (event.key.code == sf::Keyboard::Left && direction.x != 1) direction = { -1, 0 };
                else if (event.key.code == sf::Keyboard::Right && direction.x != -1) direction = { 1, 0 };
                
            }
            if (event.key.code == sf::Keyboard::R) {
                snake.clear();
                snake.push_back({ width / 2, height / 2 });
                direction = { 1, 0 };
                gameOver = false;
                do {
                    newFood.x = rand() % width;
                    newFood.y = rand() % height;
                } while (std::find(snake.begin(), snake.end(), newFood) != snake.end());
				score = 0;
            }
        }

        if (!gameOver && clock.getElapsedTime().asSeconds() > moveDelay) {
            sf::Vector2i newHead = snake.front() + direction;

            if (newHead.x < 0 || newHead.x >= width || newHead.y < 0 || newHead.y >= height) {
                gameOver = true;
            }
            for (size_t i = 1; i < snake.size(); ++i) {
                if (snake[i] == newHead) {
                    gameOver = true;
                    break;
                }
            }
            if (!gameOver) {
                snake.insert(snake.begin(), newHead);

                if (newHead.x == foodX && newHead.y == foodY) {
                    sf::Vector2i newFood;
                    do {
                        newFood.x = rand() % width;
                        newFood.y = rand() % height;
                    } while (std::find(snake.begin(), snake.end(), newFood) != snake.end());

                    foodX = newFood.x;
                    foodY = newFood.y;
					score++;
                }

                else {
                    snake.pop_back();
                }
            }
			clock.restart();
            
        }

		scoreText.setString("Score: " + std::to_string(score));

        window.clear(sf::Color::Black);
        for(auto& segment : snake) {
            cellShape.setPosition(segment.x * cellSize, segment.y * cellSize);
            cellShape.setFillColor(sf::Color::Green);
            window.draw(cellShape);
		}
        if (!gameOver) {
            cellShape.setPosition(foodX * cellSize, foodY * cellSize);
            cellShape.setFillColor(sf::Color::Red);
            window.draw(cellShape);
        }
        window.draw(scoreText);
        window.display();
    }

    return 0;
}