#pragma once

#include "Apple.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player;

class Game {
private:
	float windowWidth;
	float windowHeight;
	float gameTickSeconds;

	float playerSpeed;
	int numEatenApples;
	int gameLevel;
	int currentApplesCount;
	int applesEatenOnCurrentLevel;
	bool lockGameLevel;
	bool restartGame;
	bool gameOver;
	static const int MAX_APPLES = 20;
	Apple* apples;
	int applesCount;
	sf::Texture playerTexture;
	sf::Texture appleTexture;
	sf::SoundBuffer eatAppleSound;
	sf::SoundBuffer wallHitSound;
	sf::Sound eatApplePlayer;
	sf::Sound wallHitPlayer;
	sf::Clock gameOverDelayClock;
	bool gameOverPending;

public:
	Game();
	~Game();
	
	// Getters
	float getWindowWidth() const;
	float getWindowHeight() const;
	float getGameTickSeconds() const;
	float getPlayerSpeed() const;
	int getNumEatenApples() const;
	int getGameLevel() const;
	int getCurrentApplesCount() const;
	int getApplesEatenOnCurrentLevel() const;
	bool getLockGameLevel() const;
	bool getRestartGame() const;
	bool getGameOver() const;
	bool getGameOverPending() const;
	const Apple* getApples() const;
	Apple& getApple(int index);
	int getApplesCount() const;
	const sf::Texture& getPlayerTexture() const;
	const sf::Texture& getAppleTexture() const;
	
	// Setters
	void setWindowWidth(float width);
	void setWindowHeight(float height);
	void setGameTickSeconds(float seconds);
	void setPlayerSpeed(float speed);
	void setNumEatenApples(int count);
	void setGameLevel(int level);
	void setCurrentApplesCount(int count);
	void setApplesEatenOnCurrentLevel(int count);
	void setLockGameLevel(bool lock);
	void setRestartGame(bool restart);
	void setGameOverPending(bool pending);
	void setPlayerTexture(const sf::Texture& texture);
	void setAppleTexture(const sf::Texture& texture);
	void setGameOver(bool over);
	void setApplesCount(int count);
	// Game management
	void init();
	void addApple(float x, float y);
	void clearApples();
	void generateRandomApples();
	void drawGameOverScreen(sf::RenderWindow& window, const sf::Font& font) const;
	void updateGameUI(sf::Text& debugText, const Player& player) const;
	
	// Game loop methods
	void processWindowEvents(sf::RenderWindow& window);
	void processUserInput(Player& player);
	void updatePlayerPosition(Player& player);
	void updateGameTick(sf::Clock& gameClock, float& lastTime);
	void updateGameState(Player& player);
	void drawApples(sf::RenderWindow& window, Player& player);
	sf::Text initDebugText(const sf::Font& font) const;
	
	// Sound methods
	void playEatAppleSound();
	void playWallHitSound();
}; 