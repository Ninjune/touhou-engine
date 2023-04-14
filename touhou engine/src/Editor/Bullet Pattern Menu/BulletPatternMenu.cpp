#include <string>
#include <iostream>
#include <filesystem>
#include "BulletPatternMenu.h"
#include "../../Entity/Enemy.h"


BulletPatternMenu::BulletPatternMenu()
{
	menuRect.setFillColor(sf::Color::Transparent);
	menuRect.setOutlineColor(sf::Color::White);
	menuRect.setOutlineThickness(1);
}


void BulletPatternMenu::setPosition(float menuX, float menuY)
{
	menuRect.setPosition(menuX, menuY);
}


void BulletPatternMenu::setSize(float menuW, float menuH)
{
	menuRect.setSize(sf::Vector2f(menuW, menuH));
}


// call after setting position and size.
void BulletPatternMenu::setPatternFolder(std::string patternFolder,
	std::map<std::string, sf::Texture>& textureMap,
	sf::Font& font
)
{
	std::string patternName;

	for (std::filesystem::directory_entry file :
	std::filesystem::directory_iterator(patternFolder))
	{
		patternName = file.path().string().substr(9, file.path().string().find_first_of(".")-9);

		bulletPatterns.push_back(
			BulletPatternButton(menuRect.getPosition(),
				menuRect.getSize(),
				bulletPatterns,
				BulletPattern(patternFolder, patternName, textureMap["bullet"]),
				font
			));
	}
}


void BulletPatternMenu::update(sf::RenderWindow& window,
	int& frame,
	std::vector<Enemy>& enemies,
	int selectedEnemyIndex,
	std::map<std::string, sf::Texture>& textureMap
)
{
	for (BulletPatternButton& button : bulletPatterns)
		button.update(window, frame, enemies, selectedEnemyIndex, textureMap);
	window.draw(menuRect);
}