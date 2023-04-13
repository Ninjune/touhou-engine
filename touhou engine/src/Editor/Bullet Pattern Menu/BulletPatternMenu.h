#include "BulletPatternButton.h"
#pragma once
class BulletPatternMenu
{
public:
	BulletPatternMenu();
	void setPosition(float x, float y);
	void setSize(float width, float height);
	void setPatternFolder(std::string);
	void update(sf::RenderWindow& window);
private:
	sf::RectangleShape menuRect;
	std::vector<BulletPatternButton> bulletPatterns;
};

