#include <string>
#include <iostream>
#include <filesystem>
#include "BulletPatternMenu.h"


BulletPatternMenu::BulletPatternMenu()
{
	menuRect.setFillColor(sf::Color::Transparent);
	menuRect.setOutlineColor(sf::Color::White);
	menuRect.setOutlineThickness(2);
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
void BulletPatternMenu::setPatternFolder(std::string patternFolder)
{
	std::string patternName;
	sf::Texture bulletTexture;

	for (std::filesystem::directory_entry file :
	std::filesystem::directory_iterator(patternFolder))
	{
		patternName = file.path().string().substr(9, file.path().string().find("."));

		bulletPatterns.push_back(
			BulletPatternButton(menuRect.getSize(), menuRect.getPosition(),
				bulletPatterns, BulletPattern(patternFolder, patternName, bulletTexture)
			));
	}
}


void BulletPatternMenu::update(sf::RenderWindow& window)
{
	for (BulletPatternButton button : bulletPatterns)
		button.update(window);
	window.draw(menuRect)
}