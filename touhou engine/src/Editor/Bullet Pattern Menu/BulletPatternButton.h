#include "../../Util/BulletPattern.h"
#include "../../Entity/Enemy.h"
#pragma once

class BulletPatternButton
{
public:
	BulletPatternButton(sf::Vector2f menuPos, sf::Vector2f menuSize, 
		std::vector<BulletPatternButton> buttons, BulletPattern pattern, sf::Texture& minusTexture, sf::Texture& plusTexture);
	void update(sf::RenderWindow& window, std::vector<Enemy>& enemies, int selectedEnemyIndex);
	bool getStatus();
private:
	sf::RectangleShape mainRectangle;
	sf::RectangleShape innerRectangle;
	sf::Texture minusTexture, plusTexture;
	sf::Sprite buttonSprite;
	BulletPattern bulletPattern;
	int addedIndex;
};

