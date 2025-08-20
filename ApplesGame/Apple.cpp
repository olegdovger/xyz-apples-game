#include "Apple.h"
#include "Constants.h"
#include "Game.h"
#include "Math.h"
	
Apple::Apple() : x(0), y(0), isEaten(false) {}

Apple::Apple(float posX, float posY, const Game& game) : x(posX), y(posY), isEaten(false) {
	sprite.setTexture(game.getAppleTexture());
	Math::setSpriteSize(sprite, APPLE_SIZE);
	Math::setSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
	sprite.setPosition(x, y);
}

Apple::~Apple() {}

void Apple::setEaten(bool eaten) {
	isEaten = eaten;
}

float Apple::getX() const {
	return x;
}

float Apple::getY() const {
	return y;
}

bool Apple::getIsEaten() const {
	return isEaten;
}

sf::Sprite Apple::getShape() const {
	return sprite;
}

void Apple::updatePosition(float newX, float newY) {
	x = newX;
	y = newY;
	sprite.setPosition(x, y);
} 