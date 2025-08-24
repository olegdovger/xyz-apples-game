#pragma once

#include "Player.h"
#include "Apple.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

namespace Math {
	bool circleCollisionDetected(Position2D& playerPos, Apple& apple, float playerSize, float appleSize);
	bool rectangleCollisionDetected(Position2D& playerPos, Apple& apple, float playerSize, float appleSize);
	void setSpriteSize(sf::Sprite& sprite, float size);
	void setSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY);
	
	sf::Color applyColorAlpha(sf::Color color, unsigned char alpha);

	template<typename T>
	void insertionSortDescending(std::unordered_map<std::string, int>& map, std::vector<T>& arr, bool (*compare)(const T&, const T&)) {

		arr.reserve(map.size());
    
    	for (const auto& pair : map) {
        	arr.emplace_back(pair.first, pair.second);
    	}

		for (int i = 1; i < (int)arr.size(); ++i) {
			T key = arr[i];
			int j = i - 1;
			while (j >= 0 && compare(arr[j], key)) {
				arr[j + 1] = arr[j];
				j--;
			}
			arr[j + 1] = key;
		}
	}
} 