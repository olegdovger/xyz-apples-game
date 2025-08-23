#pragma once

#include "Apple.h"
#include "Leaderboard.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class GameState {
	SelectingModes,
	Playing,
	Dying,
	GameOver
};

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
	GameState gameState;
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
	
	int gameModes;
	Leaderboard leaderboard;

	Player* player;

public:
	Game();
	~Game();
	
	float getWindowWidth() const;
	float getWindowHeight() const;
	float getGameTickSeconds() const;
	float getPlayerSpeed() const;
	int getNumEatenApples() const;
	int getGameLevel() const;
	int getCurrentApplesCount() const;
	int getApplesEatenOnCurrentLevel() const;
	bool getLockGameLevel() const;
	GameState getGameState() const;
	const Apple* getApples() const;
	Apple& getApple(int index);
	int getApplesCount() const;
	const sf::Texture& getPlayerTexture() const;
	const sf::Texture& getAppleTexture() const;
	int getGameModes() const;
	
	void setWindowWidth(float width);
	void setWindowHeight(float height);
	void setGameTickSeconds(float seconds);
	void setPlayerSpeed(float speed);
	void setNumEatenApples(int count);
	void setGameLevel(int level);
	void setCurrentApplesCount(int count);
	void setApplesEatenOnCurrentLevel(int count);
	void setLockGameLevel(bool lock);
	void setGameState(GameState state);
	void setPlayerTexture(const sf::Texture& texture);
	void setAppleTexture(const sf::Texture& texture);
	void setApplesCount(int count);
	void setGameModes(int modes);
	void setPlayer(Player* player);
	
	void toggleGameMode(int mode);
	bool hasGameMode(int mode) const;
	void processModeSelection(sf::RenderWindow& window);
	void drawModeSelectionScreen(sf::RenderWindow& window, const sf::Font& font) const;
	void applySpeedBoost();
	
	void init();
	void addApple(float x, float y);
	void clearApples();
	void generateRandomApples();
	void drawGameOverScreen(sf::RenderWindow& window, const sf::Font& font) const;
	void updateGameUI(sf::Text& debugText) const;
	
	void processWindowEvents(sf::RenderWindow& window);
	void processUserInput();
	void updatePlayerPosition();
	void updateGameTick(sf::Clock& gameClock, float& lastTime);
	void updateGameState();
	void checkAppleCollisions();
	void updateAppleGameState();
	void drawApples(sf::RenderWindow& window);
	void resetGameState();
	sf::Text initDebugText(const sf::Font& font) const;
	
	void playEatAppleSound();
	void playWallHitSound();
}; 