#pragma once
#include <SFML/Graphics.hpp>
#include "../Entity/Bullet.h"


class BulletPattern
{
public:
	BulletPattern();
	BulletPattern(sf::Vector2f origin,
		int frequencyIn,
		int burstCountIn,
		int burstSizeIn,
		int burstSizeChangeIn,
		int directionIn,
		int directionChangeIn,
		int spawnDirectionIn,
		int spawnDirectionChangeIn,
		double velocityIn,
		double velocityChangeIn,
		std::string bulletTypeIn,
		std::string nameIn
	);
	void update(sf::RenderWindow& window,
		std::map<std::string, sf::Texture>& textureMap,
		std::vector<std::vector<std::vector<Bullet>>>& bullets,
		int& stageFrame,
		sf::Vector2f origin,
		int fireFrame
	);
	int getBulletIndex();
	int getFrequency();
	std::string getName();
private:
	sf::Vector2f origin;
	int frequency, burstCount,
		burstSize, burstSizeChange,
	    direction, directionChange,
	    spawnDirection, spawnDirectionChange;
	double velocity, velocityChange;
	int bulletsIndex;
	std::string bulletType;
	std::string name;
	std::vector<int> reservedBullets;
	bool changeWithEnemy;
};