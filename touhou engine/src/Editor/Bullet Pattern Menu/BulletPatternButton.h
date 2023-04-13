#include "../../Util/BulletPattern.h"
#pragma once

class BulletPatternButton
{
public:
	BulletPatternButton(sf::Vector2f menuPos, sf::Vector2f menuSize, 
		std::vector<BulletPatternButton> buttons, BulletPattern pattern);
	void update(sf::RenderWindow& window);
	bool getStatus();
private:
	sf::RectangleShape mainRectangle;
	sf::RectangleShape innerLine;
};

