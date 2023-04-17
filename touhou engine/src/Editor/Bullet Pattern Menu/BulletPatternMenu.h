#include "BulletPatternButton.h"
#pragma once
class BulletPatternMenu
{
public:
	BulletPatternMenu();
	void setPosition(float x, float y);
	void setSize(float width, float height);
	void setPatternFolder(std::string,
		std::map<std::string, sf::Texture>& textureMap,
		sf::Font& font
	);
	void update(sf::RenderWindow& window,
		int& frame,
		std::vector<Enemy>& enemies,
		int selectedEnemyIndex,
		std::map<std::string, sf::Texture>& textureMap
	);
private:
	Keybind m1;
	sf::RectangleShape menuRect, upButton, downButton;
	sf::Sprite upIcon, downIcon;
	std::vector<BulletPatternButton> bulletPatterns;
	std::vector<unsigned int> buttonOrder;
	int first, last;
};