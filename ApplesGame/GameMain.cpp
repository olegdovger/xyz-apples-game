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

	sf::Font debugFont;
	if (!debugFont.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Regular.ttf"))
	{
		return EXIT_FAILURE;
	}

	sf::Text gameStateText = game->initDebugText(debugFont);

	player = new Player(DEFAULT_WINDOW_WIDTH / 2.f, DEFAULT_WINDOW_HEIGHT / 2.f, INITIAL_PLAYER_SPEED, PLAYER_SIZE, *game);

	game->setPlayer(player);

	while (window.isOpen())
	{
		sf::sleep(sf::milliseconds(16));

		game->updateGameTick(gameClock, lastTime);

		if (game->getGameState() == GameState::SelectingModes) {
			game->processModeSelection(window);
			game->drawModeSelectionScreen(window, debugFont);
			window.display();
			continue;
		}

		game->processWindowEvents(window);
		game->processUserInput();

		game->updatePlayerPosition();
		game->checkAppleCollisions();
		game->updateAppleGameState();
		game->updateGameState();
		game->updateGameUI(gameStateText);

		window.clear(sf::Color(48, 48, 48));
		
		window.draw(player->getShape());
		game->drawApples(window);
		window.draw(gameStateText);

		if (game->getGameState() == GameState::GameOver) {
			game->drawGameOverScreen(window, debugFont);
		}

		window.display();
	}

	delete player;
	
	std::cout << "Press Enter to exit..." << std::endl;
	std::cin.get();
	return 0;
}
