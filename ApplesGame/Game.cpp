#include "Game.h"
#include "Constants.h"
#include "Player.h"
#include "Math.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>

Game::Game() {
	std::cout << "Game constructor started" << std::endl;
	apples = new Apple[MAX_APPLES];
	player = nullptr;
	init();
	std::cout << "Game constructor finished" << std::endl;
}

Game::~Game() {
	delete[] apples;
}

void Game::init() {
	windowWidth = DEFAULT_WINDOW_WIDTH;
	windowHeight = DEFAULT_WINDOW_HEIGHT;
	
	gameTickSeconds = 0.016f;
	playerSpeed = INITIAL_PLAYER_SPEED;
	
	numEatenApples = 0;
	gameLevel = 1;
	currentApplesCount = 0;
	applesEatenOnCurrentLevel = 0;
	applesCount = 0;
	
	lockGameLevel = false;
	gameState = GameState::SelectingModes;
	
	toggleGameMode(MODE_FINITE_APPLES);
	toggleGameMode(MODE_SPEED_BOOST);

	if (!playerTexture.loadFromFile(RESOURCES_PATH + "Player.png")) {
		std::cout << "Ошибка загрузки текстуры Player.png" << std::endl;
	} else {
		std::cout << "Текстура Player.png загружена успешно" << std::endl;
	}

	if (!appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png")) {
		std::cout << "Ошибка загрузки текстуры Apple.png" << std::endl;
	} else {
		std::cout << "Текстура Apple.png загружена успешно" << std::endl;
	}

	if (!eatAppleSound.loadFromFile(RESOURCES_PATH + "AppleEat.wav")) {
		std::cout << "Ошибка загрузки звука AppleEat.wav" << std::endl;
	} else {
		std::cout << "Звук AppleEat.wav загружен успешно" << std::endl;
		eatApplePlayer.setBuffer(eatAppleSound);
		eatApplePlayer.setVolume(10);
	}

	if (!wallHitSound.loadFromFile(RESOURCES_PATH + "Death.wav")) {
		std::cout << "Ошибка загрузки звука Death.wav" << std::endl;
	} else {
		std::cout << "Звук Death.wav загружен успешно" << std::endl;
		wallHitPlayer.setBuffer(wallHitSound);
		wallHitPlayer.setVolume(10);
	}
}

void Game::addApple(float x, float y) {
	if (applesCount < MAX_APPLES) {
		apples[applesCount] = Apple(x, y, *this);
		applesCount++;
	}
}

void Game::clearApples() {
	delete[] apples;
	apples = new Apple[MAX_APPLES];
	applesCount = 0;
}

void Game::generateRandomApples() {
	int numApples;

	if (hasGameMode(MODE_INFINITE_APPLES)) {
		numApples = rand() % 5 + 8;
	} else {
		numApples = 20;
	}
	
	setApplesEatenOnCurrentLevel(0);
	setApplesCount(0);
	
	for (int i = 0; i < numApples; i += 1) {
		float x = rand() / (float)RAND_MAX * getWindowWidth();
		float y = rand() / (float)RAND_MAX * getWindowHeight();
		addApple(x, y);
	}
	
	setCurrentApplesCount(numApples);
}


float Game::getWindowWidth() const { return windowWidth; }
float Game::getWindowHeight() const { return windowHeight; }
float Game::getGameTickSeconds() const { return gameTickSeconds; }
float Game::getPlayerSpeed() const { return playerSpeed; }
int Game::getNumEatenApples() const { return numEatenApples; }
int Game::getGameLevel() const { return gameLevel; }
int Game::getCurrentApplesCount() const { return currentApplesCount; }
int Game::getApplesEatenOnCurrentLevel() const { return applesEatenOnCurrentLevel; }
bool Game::getLockGameLevel() const { return lockGameLevel; }
GameState Game::getGameState() const { return gameState; }
const Apple* Game::getApples() const { return apples; }
Apple& Game::getApple(int index) { return apples[index]; }
int Game::getApplesCount() const { return applesCount; }
const sf::Texture& Game::getPlayerTexture() const { return playerTexture; }
const sf::Texture& Game::getAppleTexture() const { return appleTexture; }


void Game::setWindowWidth(float width) { windowWidth = width; }
void Game::setWindowHeight(float height) { windowHeight = height; }
void Game::setGameTickSeconds(float seconds) { gameTickSeconds = seconds; }
void Game::setPlayerSpeed(float speed) { playerSpeed = speed; }
void Game::setNumEatenApples(int count) { numEatenApples = count; }
void Game::setGameLevel(int level) { gameLevel = level; }
void Game::setCurrentApplesCount(int count) { currentApplesCount = count; }
void Game::setApplesEatenOnCurrentLevel(int count) { applesEatenOnCurrentLevel = count; }
void Game::setLockGameLevel(bool lock) { lockGameLevel = lock; }
void Game::setGameState(GameState state) { gameState = state; }
void Game::setPlayerTexture(const sf::Texture& texture) { playerTexture = texture; }
void Game::setAppleTexture(const sf::Texture& texture) { appleTexture = texture; }
void Game::setApplesCount(int count) { applesCount = count; }

void Game::drawGameOverScreen(sf::RenderWindow& window, const sf::Font& font) const {
	sf::RectangleShape overlay;
	overlay.setSize(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
	overlay.setFillColor(sf::Color(0, 0, 0, 180));
	window.draw(overlay);

	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("GAME OVER");
	gameOverText.setCharacterSize(80);
	gameOverText.setFillColor(Math::applyColorAlpha(sf::Color::Red, 220));
	gameOverText.setStyle(sf::Text::Bold);
	
	sf::FloatRect textRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 120);
	window.draw(gameOverText);

	sf::Text statsText;
	statsText.setFont(font);
	std::stringstream statsStream;
	statsStream << "Final Score: " << numEatenApples << " apples\n";
	statsStream << "Final Level: " << gameLevel << "\n";
	statsStream << "Final Speed: " << std::fixed << std::setprecision(1) << playerSpeed << "\n\n";
	statsStream << "Press ESC to return to mode selection";
	
	statsText.setString(statsStream.str());
	statsText.setCharacterSize(28);
	statsText.setFillColor(Math::applyColorAlpha(sf::Color::White, 200));
	
	sf::FloatRect statsRect = statsText.getLocalBounds();
	statsText.setOrigin(statsRect.left + statsRect.width / 2.0f, statsRect.top + statsRect.height / 2.0f);
	statsText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 60);
	window.draw(statsText);
}

void Game::updateGameUI(sf::Text& gameStateText) const {
	Position2D playerPos = player->getPosition();
	std::stringstream debugStream;
	debugStream << "Position: " << std::round(playerPos.x) << ", " << std::round(playerPos.y) << "\n";
	debugStream << "Speed: " << std::fixed << std::setprecision(1) << playerSpeed << "\n";
	debugStream << "Eaten apples: " << numEatenApples << "\n";
	debugStream << "Level: " << gameLevel << "\n";
	debugStream << "Level progress: " << applesEatenOnCurrentLevel << "/" << currentApplesCount << "\n";

	std::string gameModeString;
	if (hasGameMode(MODE_FINITE_APPLES)) gameModeString += "fixed apples";
	if (hasGameMode(MODE_INFINITE_APPLES)) gameModeString += "random apples";
	if (hasGameMode(MODE_SPEED_BOOST)) gameModeString += gameModeString.empty() ? "speed boost" : " and speed boost";
	if (hasGameMode(MODE_NO_SPEED_BOOST)) gameModeString += gameModeString.empty() ? "no speed boost" : " and no speed boost";

	gameStateText.setString(debugStream.str() + "Modes: " + gameModeString);
	gameStateText.setPosition(15, 15);
}

void Game::processWindowEvents(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		
		if (event.type == sf::Event::Resized) {
			sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
			window.setView(sf::View(visibleArea));

			setWindowWidth((float)event.size.width);
			setWindowHeight((float)event.size.height);
		}
	}
}

void Game::processUserInput() {
	if (getGameState() == GameState::GameOver) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			setGameState(GameState::SelectingModes);
			resetGameState();
			
		}
		return;
	}
	
	if (getGameState() == GameState::SelectingModes) {
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player->setDirection(PlayerDirection::Right);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player->setDirection(PlayerDirection::Left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player->setDirection(PlayerDirection::Up);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player->setDirection(PlayerDirection::Down);
	}
}

void Game::updatePlayerPosition() {
	bool shouldRestart = player->update(getGameTickSeconds(), getWindowWidth(), getWindowHeight());
	
	if (shouldRestart) {
		if (getGameState() == GameState::Playing) {
			playWallHitSound();
			setGameState(GameState::Dying);
		}
	}
}

void Game::updateGameTick(sf::Clock& gameClock, float& lastTime) {
	float currentTime = gameClock.getElapsedTime().asSeconds();
	setGameTickSeconds(currentTime - lastTime);
	lastTime = currentTime;
}

void Game::updateGameState() {
	if (getGameState() == GameState::Dying) {
		if (gameOverDelayClock.getElapsedTime().asSeconds() >= 1.0f) {
			setGameState(GameState::GameOver);
			gameOverDelayClock.restart();
		}
	}
}

void Game::checkAppleCollisions() {
	Position2D playerPos = player->getPosition();
	
	for (int i = 0; i < getApplesCount(); i += 1) {
		Apple& apple = getApple(i);
		if (apple.getIsEaten() == false) {
			if (Math::circleCollisionDetected(playerPos, apple, PLAYER_SIZE, APPLE_SIZE)) {
				apple.setEaten(true);

				applesEatenOnCurrentLevel += 1;
				numEatenApples += 1;

				applySpeedBoost();

				if (hasGameMode(MODE_NO_SPEED_BOOST) == false) {
					playerSpeed += PLAYER_SPEED_DELTA;
					player->setSpeed(playerSpeed);
				}

				playEatAppleSound();
			}
		}
	}
}

void Game::updateAppleGameState() {
	if (getApplesEatenOnCurrentLevel() >= getCurrentApplesCount() && getLockGameLevel() == false) {
		setGameLevel(getGameLevel() + 1);
		setLockGameLevel(true);
		
		clearApples();
		generateRandomApples();
	}

	if (getApplesEatenOnCurrentLevel() < getCurrentApplesCount() && getLockGameLevel() == true) {
		setLockGameLevel(false);
	}
}

void Game::drawApples(sf::RenderWindow& window) {
	for (int i = 0; i < getApplesCount(); i += 1) {
		if (!getApple(i).getIsEaten()) {
			window.draw(getApple(i).getShape());
		}
	}
}

void Game::resetGameState() {
	setGameLevel(1);
	setPlayerSpeed(INITIAL_PLAYER_SPEED);
	player->reset(getWindowWidth() / 2.f, getWindowHeight() / 2.f, INITIAL_PLAYER_SPEED);
	setNumEatenApples(0);
	clearApples();
	setCurrentApplesCount(0);
	setApplesEatenOnCurrentLevel(0);
	setLockGameLevel(false);

	setGameModes(MODE_FINITE_APPLES | MODE_SPEED_BOOST);

	gameOverDelayClock.restart();
}

sf::Text Game::initDebugText(const sf::Font& font) const {
	sf::Text debugText;
	debugText.setFont(font);
	debugText.setCharacterSize(16);
	debugText.setFillColor(Math::applyColorAlpha(sf::Color::White, 180));
	debugText.setPosition(15, 15);
	return debugText;
}

void Game::playEatAppleSound() {
	eatApplePlayer.play();
}

void Game::playWallHitSound() {
	wallHitPlayer.play();
} 


int Game::getGameModes() const { return gameModes; }

void Game::setGameModes(int modes) { gameModes = modes; }


void Game::toggleGameMode(int mode) {
	if (mode == MODE_FINITE_APPLES && hasGameMode(MODE_INFINITE_APPLES)) {
		gameModes &= ~MODE_INFINITE_APPLES;
	}
	if (mode == MODE_INFINITE_APPLES && hasGameMode(MODE_FINITE_APPLES)) {
		gameModes &= ~MODE_FINITE_APPLES;
	}
	if (mode == MODE_SPEED_BOOST && hasGameMode(MODE_NO_SPEED_BOOST)) {
		gameModes &= ~MODE_NO_SPEED_BOOST;
	}
	if (mode == MODE_NO_SPEED_BOOST && hasGameMode(MODE_SPEED_BOOST)) {
		gameModes &= ~MODE_SPEED_BOOST;
	}
	
	gameModes ^= mode;
}

bool Game::hasGameMode(int mode) const {
	return (gameModes & mode) != 0;
}

void Game::processModeSelection(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		else if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
				case sf::Keyboard::Num1:
					toggleGameMode(MODE_FINITE_APPLES);
					break;
				case sf::Keyboard::Num2:
					toggleGameMode(MODE_INFINITE_APPLES);
					break;
				case sf::Keyboard::Num3:
					toggleGameMode(MODE_SPEED_BOOST);
					break;
				case sf::Keyboard::Num4:
					toggleGameMode(MODE_NO_SPEED_BOOST);
					break;
				case sf::Keyboard::Enter:
					if (gameModes == 0) {
						gameModes = MODE_FINITE_APPLES | MODE_SPEED_BOOST;
					}
					setGameState(GameState::Playing);
					generateRandomApples();
					break;
			}
		}
	}
}

void Game::drawModeSelectionScreen(sf::RenderWindow& window, const sf::Font& font) const {
	window.clear(sf::Color(48, 48, 48));
	
	sf::Text titleText;
	titleText.setFont(font);
	titleText.setString("Select Game Mode");
	titleText.setCharacterSize(36);
	titleText.setFillColor(Math::applyColorAlpha(sf::Color::White, 200));
	titleText.setPosition(200, 50);
	window.draw(titleText);
	
	std::vector<std::string> options = {
		"1 - Fixed amount of apples (20)",
		"2 - Random amount of apples", 
		"3 - Speed boost after eating apple",
		"4 - No speed boost",
		"",
		"Enter - Start game"
	};
	
	std::vector<int> modes = {MODE_FINITE_APPLES, MODE_INFINITE_APPLES, MODE_SPEED_BOOST, MODE_NO_SPEED_BOOST};
	
	for (int i = 0; i < options.size(); ++i) {
		sf::Text optionText;
		optionText.setFont(font);
		optionText.setString(options[i]);
		optionText.setCharacterSize(22);
		optionText.setPosition((float)200, (float)(120 + i * 45));
		
		if (i < modes.size()) {
			if (hasGameMode(modes[i])) {
				optionText.setFillColor(Math::applyColorAlpha(sf::Color::Green, 200));
			} else {
				optionText.setFillColor(Math::applyColorAlpha(sf::Color::White, 180));
			}
		} else {
			optionText.setFillColor(Math::applyColorAlpha(sf::Color::Yellow, 200));
		}
		
		window.draw(optionText);
	}

	std::string modesString;
	if (hasGameMode(MODE_FINITE_APPLES)) modesString += "Fixed amount";
	if (hasGameMode(MODE_INFINITE_APPLES)) modesString += "Random amount";
	if (hasGameMode(MODE_SPEED_BOOST)) modesString += modesString.empty() ? "Speed boost" : " and speed boost";
	if (hasGameMode(MODE_NO_SPEED_BOOST)) modesString += modesString.empty() ? "No speed boost" : " and no speed boost";
	
	if (modesString.empty()) {
		modesString = "No modes selected - Press 1,2,3,4 to choose";
	}
	
	sf::Text modesText;
	modesText.setFont(font);
	modesText.setString(modesString);
	modesText.setCharacterSize(18);
	modesText.setFillColor(Math::applyColorAlpha(sf::Color::Yellow, 180));
	modesText.setPosition(200, 380);
	window.draw(modesText);
}

void Game::applySpeedBoost() {
	if (hasGameMode(MODE_SPEED_BOOST) && !hasGameMode(MODE_NO_SPEED_BOOST)) {
		if (numEatenApples % APPLES_FOR_SPEED_BOOST == 0 && numEatenApples > 0) {
			playerSpeed *= SPEED_BOOST_FACTOR;
			std::cout << "Скорость увеличена! Текущая скорость: " << playerSpeed << std::endl;
		}
	}
}

void Game::setPlayer(Player* player) {
	this->player = player;
} 