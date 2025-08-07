#include "Game.h"
#include "Constants.h"
#include "Player.h"
#include "Math.h"
#include <cassert>
#include <iostream>
#include <sstream>

Game::Game() {
	std::cout << "Game constructor started" << std::endl;
	apples = new Apple[MAX_APPLES];
	init();
	std::cout << "Game constructor finished" << std::endl;
}

Game::~Game() {
	// Деструктор для очистки ресурсов
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
	restartGame = false;
	gameOver = false;
	gameOverPending = false;

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
	}

	if (!wallHitSound.loadFromFile(RESOURCES_PATH + "Death.wav")) {
		std::cout << "Ошибка загрузки звука Death.wav" << std::endl;
	} else {
		std::cout << "Звук Death.wav загружен успешно" << std::endl;
		wallHitPlayer.setBuffer(wallHitSound);
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
	int numApples = rand() % 2 + 5; // Генерируем от 5 до 14 яблок
	setCurrentApplesCount(numApples);
	setApplesEatenOnCurrentLevel(0);
	setApplesCount(0); // Сбрасываем счетчик яблок в массиве
	for (int i = 0; i < numApples; i += 1) {
		float x = rand() / (float)RAND_MAX * getWindowWidth();
		float y = rand() / (float)RAND_MAX * getWindowHeight();
		addApple(x, y);
	}
}

// Getters
float Game::getWindowWidth() const { return windowWidth; }
float Game::getWindowHeight() const { return windowHeight; }
float Game::getGameTickSeconds() const { return gameTickSeconds; }
float Game::getPlayerSpeed() const { return playerSpeed; }
int Game::getNumEatenApples() const { return numEatenApples; }
int Game::getGameLevel() const { return gameLevel; }
int Game::getCurrentApplesCount() const { return currentApplesCount; }
int Game::getApplesEatenOnCurrentLevel() const { return applesEatenOnCurrentLevel; }
bool Game::getLockGameLevel() const { return lockGameLevel; }
bool Game::getRestartGame() const { return restartGame; }
bool Game::getGameOver() const { return gameOver; }
bool Game::getGameOverPending() const { return gameOverPending; }
const Apple* Game::getApples() const { return apples; }
Apple& Game::getApple(int index) { return apples[index]; }
int Game::getApplesCount() const { return applesCount; }
const sf::Texture& Game::getPlayerTexture() const { return playerTexture; }
const sf::Texture& Game::getAppleTexture() const { return appleTexture; }

// Setters
void Game::setWindowWidth(float width) { windowWidth = width; }
void Game::setWindowHeight(float height) { windowHeight = height; }
void Game::setGameTickSeconds(float seconds) { gameTickSeconds = seconds; }
void Game::setPlayerSpeed(float speed) { playerSpeed = speed; }
void Game::setNumEatenApples(int count) { numEatenApples = count; }
void Game::setGameLevel(int level) { gameLevel = level; }
void Game::setCurrentApplesCount(int count) { currentApplesCount = count; }
void Game::setApplesEatenOnCurrentLevel(int count) { applesEatenOnCurrentLevel = count; }
void Game::setLockGameLevel(bool lock) { lockGameLevel = lock; }
void Game::setRestartGame(bool restart) { restartGame = restart; }
void Game::setGameOverPending(bool pending) { gameOverPending = pending; }
void Game::setGameOver(bool over) { gameOver = over; }
void Game::setPlayerTexture(const sf::Texture& texture) { playerTexture = texture; }
void Game::setAppleTexture(const sf::Texture& texture) { appleTexture = texture; }
void Game::setApplesCount(int count) { applesCount = count; }

void Game::drawGameOverScreen(sf::RenderWindow& window, const sf::Font& font) const {
	// Создаем полупрозрачный фон
	sf::RectangleShape overlay;
	overlay.setSize(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
	overlay.setFillColor(sf::Color(0, 0, 0, 128));
	window.draw(overlay);

	// Текст "GAME OVER"
	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("GAME OVER");
	gameOverText.setCharacterSize(72);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setStyle(sf::Text::Bold);
	
	// Центрируем текст
	sf::FloatRect textRect = gameOverText.getLocalBounds();
	gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 100);
	window.draw(gameOverText);

	// Статистика игры
	sf::Text statsText;
	statsText.setFont(font);
	std::stringstream statsStream;
	statsStream << "Final Score: " << numEatenApples << " apples\n";
	statsStream << "Final Level: " << gameLevel << "\n";
	statsStream << "Final Speed: " << playerSpeed << "\n\n";
	statsStream << "Press ESC to restart";
	
	statsText.setString(statsStream.str());
	statsText.setCharacterSize(24);
	statsText.setFillColor(sf::Color::White);
	
	// Центрируем статистику
	sf::FloatRect statsRect = statsText.getLocalBounds();
	statsText.setOrigin(statsRect.left + statsRect.width / 2.0f, statsRect.top + statsRect.height / 2.0f);
	statsText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50);
	window.draw(statsText);
}

void Game::updateGameUI(sf::Text& debugText, const Player& player) const {
	// Update debug text
	Position2D playerPos = player.getPosition();
	std::stringstream debugStream;
	debugStream << "Position: " << std::round(playerPos.x) << ", " << std::round(playerPos.y) << "\n";
	debugStream << "Speed: " << playerSpeed << "\n";
	debugStream << "Eaten apples: " << numEatenApples << "\n";
	debugStream << "Level: " << gameLevel << "\n";
	debugStream << "Current level: " << applesEatenOnCurrentLevel << "/" << currentApplesCount << "\n";

	debugText.setString(debugStream.str());
	debugText.setPosition(10, 10);
}

void Game::processWindowEvents(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		
		if (event.type == sf::Event::Resized) {
			// Update viewport when window is resized
			sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
			window.setView(sf::View(visibleArea));

			setWindowWidth((float)event.size.width);
			setWindowHeight((float)event.size.height);
		}
	}
}

void Game::processUserInput(Player& player) {
	if (getGameOver()) {
		// В режиме Game Over обрабатываем только ESC для перезапуска
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			setGameOver(false);
			setRestartGame(true);
		}
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.setDirection(PlayerDirection::Right);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.setDirection(PlayerDirection::Left);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.setDirection(PlayerDirection::Up);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.setDirection(PlayerDirection::Down);
	}
}

void Game::updatePlayerPosition(Player& player) {
	if (player.update(getGameTickSeconds(), getWindowWidth(), getWindowHeight())) {
		if (!getGameOverPending()) {
			playWallHitSound();
			setGameOverPending(true);
		}
	}
}

void Game::updateGameTick(sf::Clock& gameClock, float& lastTime) {
	float currentTime = gameClock.getElapsedTime().asSeconds();
	setGameTickSeconds(currentTime - lastTime);
	lastTime = currentTime;
}

void Game::updateGameState(Player& player) {
	// Обработка задержки перед Game Over
	if (getGameOverPending() && !getGameOver()) {
		if (gameOverDelayClock.getElapsedTime().asSeconds() >= 1.0f) { // 1 секунда задержки
			setGameOver(true);
			setGameOverPending(false);
			wallHitPlayer.stop();
			gameOverDelayClock.restart();
		}
	}

	if (getRestartGame()) {
		setRestartGame(false);
		setGameLevel(1);
		setPlayerSpeed(INITIAL_PLAYER_SPEED);
		player.reset(getWindowWidth() / 2.f, getWindowHeight() / 2.f, INITIAL_PLAYER_SPEED);
		setNumEatenApples(0);
		clearApples(); // Очищаем массив яблок при перезапуске
		setCurrentApplesCount(0);
		setApplesEatenOnCurrentLevel(0);
		setGameOverPending(false);
		generateRandomApples();
	}
}

void Game::drawApples(sf::RenderWindow& window, Player& player) {
	Position2D playerPos = player.getPosition();
	int drawnApples = 0;
	const Apple* apples = getApples();
	
	for (int i = 0; i < getApplesCount(); i += 1) {
		if (!getApple(i).getIsEaten()) {
			drawnApples++;

			if (Math::circleCollisionDetected(playerPos, getApple(i), PLAYER_SIZE, APPLE_SIZE)) {
				getApple(i).setEaten(true);
				setNumEatenApples(getNumEatenApples() + 1);
				setApplesEatenOnCurrentLevel(getApplesEatenOnCurrentLevel() + 1);
				setPlayerSpeed(getPlayerSpeed() + PLAYER_SPEED_DELTA);
				player.setSpeed(getPlayerSpeed());
				playEatAppleSound();
			}

			window.draw(apples[i].getShape());
		}
	}
	
	if (getApplesEatenOnCurrentLevel() >= getCurrentApplesCount() && getLockGameLevel() == false) {
		clearApples(); // Очищаем массив съеденных яблок
		generateRandomApples(); // Генерируем новые яблоки
		setGameLevel(getGameLevel() + 1);
		setLockGameLevel(true);
	}

	if (getApplesEatenOnCurrentLevel() < getCurrentApplesCount() && getLockGameLevel() == true) {
		setLockGameLevel(false);
	}
}

sf::Text Game::initDebugText(const sf::Font& font) const {
	sf::Text debugText;
	debugText.setFont(font);
	debugText.setCharacterSize(14);
	debugText.setFillColor(sf::Color::White);
	debugText.setPosition(10, 10);
	return debugText;
}

void Game::playEatAppleSound() {
	eatApplePlayer.setVolume(50);
	eatApplePlayer.play();
}

void Game::playWallHitSound() {
	wallHitPlayer.setVolume(50);
	wallHitPlayer.play();
} 