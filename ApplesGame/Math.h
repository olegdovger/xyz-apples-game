#pragma once

#include "Player.h"
#include "Apple.h"
#include <SFML/Graphics.hpp>

namespace Math {
	bool circleCollisionDetected(Position2D& playerPos, Apple& apple, float playerSize, float appleSize);
	bool rectangleCollisionDetected(Position2D& playerPos, Apple& apple, float playerSize, float appleSize);
	void setSpriteSize(sf::Sprite& sprite, float size);
	void setSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY);
	
	sf::Color applyColorAlpha(sf::Color color, unsigned char alpha);
} 