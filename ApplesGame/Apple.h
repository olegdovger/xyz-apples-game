#pragma once

#include <SFML/Graphics.hpp>

class Game;

class Apple {
private:
	float x;
	float y;
	bool isEaten;
	sf::Sprite sprite;

public:
	Apple();
	Apple(float posX, float posY, const Game& game);
	~Apple();

	void setEaten(bool eaten);
	
	float getX() const;
	float getY() const;
	bool getIsEaten() const;
	sf::Sprite getShape() const;
	
	void updatePosition(float newX, float newY);
}; 