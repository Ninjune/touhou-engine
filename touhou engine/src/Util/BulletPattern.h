#pragma once
#include <SFML/Graphics.hpp>

class BulletPattern
{
public:
	BulletPattern(std::string patternFolder, std::string fileName, sf::Texture& bulletTexture);
	std::string getName();
private:
	sf::Vector2f origin;
	int frequency, burstCount,
		burstSize, burstSizeChange,
	    direction, directionChange,
	    spawnDirection, spawnDirectionChange;
	double velocity, velocityChange;
	std::string bulletType;
	std::string name;
};