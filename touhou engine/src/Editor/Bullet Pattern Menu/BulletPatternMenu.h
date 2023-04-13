#include "BulletPatternButton.h"
#pragma once
class BulletPatternMenu
{
public:
	BulletPatternMenu();
	void setPosition(float x, float y);
	void setSize(float width, float height);
	void setPatternFolder(std::string);
	void update(sf::RenderWindow& window, std::vector<Enemy>& enemies, int selectedEnemyIndex);
private:
	sf::RectangleShape menuRect;
	sf::Texture minusTexture, plusTexture, bulletTexture;
	std::vector<BulletPatternButton> bulletPatterns;
};

