#pragma once

#include <SFML/Graphics.hpp>

struct Position2D {
	float x = 0;
	float y = 0;
};

enum class PlayerDirection : int {
	None = 0,
	Left = 1,
	Right,
	Up,
	Down
};

class Game;

class Player {
private:
	Position2D position;
	PlayerDirection direction;
	float speed;
	float size;
	sf::Sprite sprite;
	
	void updateSpriteRotation();

public:
	Player(float x, float y, float initialSpeed, float playerSize, const Game& game);
	~Player();
	
	bool update(float deltaTime, float windowWidth, float windowHeight);
	void setDirection(PlayerDirection newDirection);
	void setSpeed(float newSpeed);
	
	Position2D getPosition() const;
	float getSpeed() const;
	float getSize() const;
	PlayerDirection getDirection() const;
	
	sf::Sprite getShape() const;
	
	void reset(float x, float y, float initialSpeed);
}; 