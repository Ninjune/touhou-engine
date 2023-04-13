#pragma once
#include <SFML/Graphics.hpp>

class BulletPattern
{
public:

private:
	sf::Vector2f origin;
	int frequency, burstCount,
		burstSize, burstSizeChange,
	    direction, directionChange,
	    spawnDirection, spawnDirectionChange;
	double velocity, velocityChange;
	std::string bulletType;
};