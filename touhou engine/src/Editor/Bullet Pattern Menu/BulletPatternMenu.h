#include "BulletPatternButton.h"
#include "../../Entity/Enemy.h"

#pragma once
class BulletPatternMenu
{
public:
	BulletPatternMenu();
	void setPosition(float x, float y);
	void setSize(float width, float height);
	void setPatternFolder(std::string,
		std::map<std::string, sf::Texture>& textureMap,
		sf::Font& font,
		std::map<std::string, BulletPattern>& patterns
	);
	void update(sf::RenderWindow& window,
		int& frame,
		std::vector<Enemy>& enemies,
		int& selectedEnemyIndex,
		std::map<std::string, sf::Texture>& textureMap,
		int currentFrame,
		bool& patternChanger
	);
private:
	Keybind m1;
	sf::RectangleShape menuRect, upButton, downButton;
	MenuButton addPatternButton;
	sf::Sprite upIcon, downIcon;
	std::vector<BulletPatternButton> buttons;
	std::vector<unsigned int> buttonOrder;
	int first, last;
};