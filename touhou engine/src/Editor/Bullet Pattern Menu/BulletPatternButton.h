#include "../../Util/BulletPattern.h"
#include "../../Entity/Enemy.h"
#include "../../Util/Keybind.h"
#pragma once

class BulletPatternButton
{
public:
	BulletPatternButton(sf::Vector2f menuPos,
		sf::Vector2f menuSize,
		std::vector<BulletPatternButton> buttons,
		BulletPattern pattern,
		sf::Font& font
	);
	void update(sf::RenderWindow& window,
		int& frame,
		std::vector<Enemy>& enemies,
		int selectedEnemyIndex,
		std::map<std::string, sf::Texture>& textureMap
	);
	bool getStatus();
	std::string getName();
	void moveUp();
	void moveDown();
private:
	sf::Text patternName;
	Keybind m1;
	sf::RectangleShape mainRectangle;
	sf::RectangleShape innerRectangle;
	sf::Sprite buttonSprite;
	BulletPattern bulletPattern;
	bool added;
};

