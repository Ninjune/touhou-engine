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
		int fireFrame,
		std::vector<sf::Vector2f> path,
		sf::RectangleShape playableArea,
		int startFrame,
		bool pather = false
	);
	std::string getName();
	// these are public because im not making a setter/getter for all of these.
	std::map<std::string, float> getOptions();
	void setOptions(std::map<std::string, float>);
private:
	int frequency, burstCount,
		burstSize, burstSizeChange,
		direction, directionChange,
		spawnDirection, spawnDirectionChange;
	double velocity, velocityChange;
	sf::Vector2f playableToPather(sf::Vector2f point,
		sf::RenderWindow& window,
		sf::RectangleShape& playableArea
	);
	sf::Vector2f origin;
	int bulletsIndex;
	std::map<std::string, float> options;
	std::string bulletType;
	std::string name;
	std::vector<int> reservedBullets;
	bool changeWithEnemy;
};