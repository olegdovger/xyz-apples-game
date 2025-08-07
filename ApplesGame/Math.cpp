#include "Math.h"
#include <cmath>

namespace Math {
	bool circleCollisionDetected(Position2D& playerPos, Apple& apple, float playerSize, float appleSize) {
		float distance = (playerPos.x - apple.getX()) * (playerPos.x - apple.getX()) + (playerPos.y - apple.getY()) * (playerPos.y - apple.getY());
		float radiusDistance = (playerSize + appleSize) * (playerSize + appleSize) / 4.f;

		return distance <= radiusDistance;
	}

	bool rectangleCollisionDetected(Position2D& playerPos, Apple& apple, float playerSize, float appleSize) {
		float deltaX = fabs(playerPos.x - apple.getX());
		float deltaY = fabs(playerPos.y - apple.getY());

		return (deltaX <= (appleSize + playerSize) / 2.f &&
			deltaY <= (appleSize + playerSize) / 2.f);
	}

	void setSpriteSize(sf::Sprite& sprite, float size) {
		sf::FloatRect bounds = sprite.getLocalBounds();
		
		sprite.setScale(size / bounds.width, size / bounds.height);
	}

	void setSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY) {
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin(bounds.width * originX, bounds.height * originY);
	}
} 