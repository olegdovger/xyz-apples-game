#include "Player.h"
#include "Game.h"
#include "Math.h"
#include <iostream>

Player::Player(float x, float y, float initialSpeed, float playerSize, const Game& game) 
	: position{x, y}, direction(PlayerDirection::None), speed(initialSpeed), size(playerSize) {

	sprite.setTexture(game.getPlayerTexture());
	Math::setSpriteSize(sprite, size);
	Math::setSpriteRelativeOrigin(sprite, 0.5f, 0.5f);
	updateSpriteRotation();
}

Player::~Player() {
	// Деструктор для очистки ресурсов
}

bool Player::update(float deltaTime, float windowWidth, float windowHeight) {
	float positionDelta = speed * deltaTime;
	bool shouldRestart = false;

	switch (direction) {
	case PlayerDirection::Right:
		if (position.x + positionDelta + size / 2.f <= windowWidth) {
			position.x += positionDelta;
		} else {
			position.x = windowWidth - size / 2.f;
			shouldRestart = true;
		}
		break;
	case PlayerDirection::Left:
		if (position.x - positionDelta - size / 2.f >= 0) {
			position.x -= positionDelta;
		} else {
			position.x = size / 2.f;
			shouldRestart = true;
		}
		break;
	case PlayerDirection::Up:
		if (position.y - positionDelta - size / 2.f >= 0) {
			position.y -= positionDelta;
		} else {
			position.y = size / 2.f;
			shouldRestart = true;
		}
		break;
	case PlayerDirection::Down:
		if (position.y + positionDelta + size / 2.f <= windowHeight) {
			position.y += positionDelta;
		} else {
			position.y = windowHeight - size / 2.f;
			shouldRestart = true;
		}
		break;
    case PlayerDirection::None:
        break;
	}
	
	// Update sprite position when position changes
	sprite.setPosition(position.x, position.y);
	
	return shouldRestart;
}

void Player::setDirection(PlayerDirection newDirection) {
	direction = newDirection;
	updateSpriteRotation();
}

void Player::setSpeed(float newSpeed) {
	speed = newSpeed;
}

Position2D Player::getPosition() const {
	return position;
}

float Player::getSpeed() const {
	return speed;
}

float Player::getSize() const {
	return size;
}

PlayerDirection Player::getDirection() const {
	return direction;
}

sf::Sprite Player::getShape() const {
	// Отладочная информация
	static int debugCount = 0;
	if (debugCount % 60 == 0) {
		std::cout << "Player position: (" << position.x << ", " << position.y << "), size: " << size << std::endl;
	}
	debugCount++;
	
	return sprite;
}

void Player::reset(float x, float y, float initialSpeed) {
	position.x = x;
	position.y = y;
	speed = initialSpeed;
	direction = PlayerDirection::None;
	sprite.setPosition(position.x, position.y);
	updateSpriteRotation();
}

void Player::updateSpriteRotation() {
	switch (direction) {
	case PlayerDirection::Right:
		sprite.setRotation(0.f); // По умолчанию смотрит направо
		sprite.setScale(abs(sprite.getScale().x), abs(sprite.getScale().y)); // Нормальный масштаб
		break;
	case PlayerDirection::Left:
		sprite.setRotation(0.f); // Оставляем поворот как есть
		sprite.setScale(-abs(sprite.getScale().x), abs(sprite.getScale().y)); // Инвертируем по горизонтали
		break;
	case PlayerDirection::Up:
		sprite.setRotation(-90.f); // Поворот на -90 градусов
		sprite.setScale(abs(sprite.getScale().x), abs(sprite.getScale().y)); // Нормальный масштаб
		break;
	case PlayerDirection::Down:
		sprite.setRotation(90.f); // Поворот на 90 градусов
		sprite.setScale(abs(sprite.getScale().x), abs(sprite.getScale().y)); // Нормальный масштаб
		break;
	case PlayerDirection::None:
		// Оставляем текущий поворот и масштаб
		sprite.setRotation(0.f);
		sprite.setScale(abs(sprite.getScale().x), abs(sprite.getScale().y));
		break;
	}
}