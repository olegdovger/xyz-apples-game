// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Constants.h"
#include "Game.h"

Game* game = nullptr;
Player* player = nullptr;

sf::Clock gameClock;
float lastTime = gameClock.getElapsedTime().asSeconds();

int main()
{
	game = new Game();

	sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Apples game");

	int seed = (int)time(nullptr);
	srand(seed);

	game->clearApples();
	game->generateRandomApples();

	// Debug text setup
	sf::Font debugFont;
	if (!debugFont.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"))
	{
		return EXIT_FAILURE;
	}

	sf::Text debugText = game->initDebugText(debugFont);

	// Инициализируем игрока после создания окна и загрузки шрифта
	player = new Player(DEFAULT_WINDOW_WIDTH / 2.f, DEFAULT_WINDOW_HEIGHT / 2.f, INITIAL_PLAYER_SPEED, PLAYER_SIZE, *game);

	while (window.isOpen())
	{
		sf::sleep(sf::milliseconds(16));

		game->updateGameTick(gameClock, lastTime);

        game->processWindowEvents(window);
		game->processUserInput(*player);

		game->updatePlayerPosition(*player);
		game->updateGameState(*player);
		game->updateGameUI(debugText, *player);

		window.clear(sf::Color(48, 48, 48)); // Grey color
		
		window.draw(player->getShape());
		game->drawApples(window, *player);
		window.draw(debugText);

		if (game->getGameOver()) {
			// Экран Game Over поверх игровых объектов
			game->drawGameOverScreen(window, debugFont);
		}

		window.display();
	}

	delete player;
	
	std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();
	return 0;
}
