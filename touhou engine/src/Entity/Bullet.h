#include <SFML/Graphics.hpp>
#include "Character.h"
#pragma once

class Bullet : public Character
{
public:
	Bullet(sf::Texture&, int, float, sf::Vector2f, bool);
	Bullet(sf::Texture&, int, float, sf::Vector2f);
	void updateSprite(sf::RenderWindow& window, int frame);
	float getBulletSpeed();
	void setBulletSpeed(float);
	int getBulletType();
	void setBulletType(int);
	bool isPlayerOwned();
private:
	void setup(sf::Texture&, int, float, sf::Vector2f, bool);
	bool first;
	int aliveFrames, bulletType; // 0 = player, 
	float xOffset, bulletSpeed; // negative = go up, pos = go down
	bool playerOwned;
	sf::Vector2f spawnPosition;
};