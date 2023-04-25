#include <SFML/Graphics.hpp>
#include "Character.h"
#pragma once

class Bullet : public Character
{
public:
	Bullet();
	Bullet(std::map<std::string, sf::Texture>& textureMap,
		sf::Vector2f originIn,
		int directionIn,
		int spawnDirectionIn,
		double velocityIn,
		std::string bulletTypeIn,
		bool playerOwned = false
	);
	void updateSprite(sf::RenderWindow& window, int frame);
	float getBulletSpeed();
	void setBulletSpeed(float);
	int getBulletType();
	void setBulletType(int);
	bool isPlayerOwned();
private:
	void setup(std::map<std::string, sf::Texture>& textureMap,
		sf::Vector2f originIn,
		int directionIn,
		int spawnDirectionIn,
		double velocityIn,
		std::string bulletTypeIn,
		bool playerOwned
	);
	int aliveFrames, bulletType; // 0 = player, 
	float xOffset, bulletSpeed; // negative = go up, pos = go down
	bool playerOwned;
};