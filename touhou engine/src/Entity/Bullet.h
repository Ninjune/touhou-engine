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
	void simulateFrames(sf::RenderWindow& window,
		int simulatedFrames,
		sf::Vector2f originIn,
		sf::Vector2f playerPosIn,
		bool& inBounds
	);
	void adjust(std::vector<sf::Vector2f> path, int frequency, int num);
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
	int aliveFrames, bulletType, direction;
	float xOffset, velocity; // negative = go up, pos = go down
	bool playerOwned, firstFrame;
	sf::Vector2f origin, playerPos; // playerPos is the playerPos when bullet created
};