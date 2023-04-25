#include <string>
#include <filesystem>
#include "BulletPatternMenu.h"
#include "../../Entity/Enemy.h"


BulletPatternMenu::BulletPatternMenu()
	: m1(sf::Mouse::Left)
{
	menuRect.setFillColor(sf::Color::Transparent);
	menuRect.setOutlineColor(sf::Color::White);
	menuRect.setOutlineThickness(1);

	upButton.setSize(sf::Vector2f(32, 32));
	upButton.setOrigin(upButton.getSize().x / 2, downButton.getSize().y / 2);
	upButton.setOutlineColor(sf::Color::White);
	upButton.setOutlineThickness(1);
	upButton.setFillColor(sf::Color::Transparent);

	downButton.setSize(sf::Vector2f(32, 32));
	downButton.setOrigin(downButton.getSize().x / 2, downButton.getSize().y / 2);
	downButton.setOutlineColor(sf::Color::White);
	downButton.setOutlineThickness(1);
	downButton.setFillColor(sf::Color::Transparent);

	first = 0;
	last = 8;
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
	sf::Font& font,
	std::map<std::string, BulletPattern>& patterns
)
{
	std::string patternName;
	upButton.setPosition(menuRect.getPosition().x + menuRect.getSize().x/2, 
		menuRect.getPosition().y
	);
	downButton.setPosition(menuRect.getPosition().x + menuRect.getSize().x / 2,
		menuRect.getPosition().y + menuRect.getSize().y - downButton.getSize().y/2
	);

	upIcon.setTexture(textureMap["upIcon"]);
	upIcon.setOrigin(256, 0);
	upIcon.setScale(32. / 512, 32. / 512);
	upIcon.setPosition(upButton.getPosition().x, upButton.getPosition().y);

	downIcon.setTexture(textureMap["upIcon"]);
	downIcon.setRotation(180);
	downIcon.setOrigin(256, 256);
	downIcon.setScale(32. / 512, 32. / 512);
	downIcon.setPosition(downButton.getPosition().x, downButton.getPosition().y);

	unsigned int count = 0;
	for (std::map<std::string, BulletPattern>::iterator iterator = patterns.begin();
	iterator != patterns.end(); iterator++)
	{
		patternName = iterator->second.getName();

		buttons.push_back(
			BulletPatternButton(menuRect.getPosition(),
				menuRect.getSize(),
				buttons,
				patterns[patternName],
				font
		));

		buttonOrder.push_back(count++);
	}
	unsigned int temp;

	for (unsigned int i = 0; i < buttons.size() - 1; i++)
	{
		for (unsigned int j = 0; j < buttons.size() - i - 1; j++)
		{
			if (buttons[buttonOrder[j]].getName() > buttons[buttonOrder[j + 1]].getName())
			{
				temp = buttonOrder[j];
				buttonOrder[j] = buttonOrder[j - 1];
				buttonOrder[j - 1] = temp;
			}
		}
	}
}

void BulletPatternMenu::update(sf::RenderWindow& window,
	int& frame,
	std::vector<Enemy>& enemies,
	int& selectedEnemyIndex,
	std::map<std::string, sf::Texture>& textureMap,
	int currentFrame
)
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, 
		sf::Mouse::getPosition(window).y
	);

	if (m1.consumeClick(frame, 4))
	{
		if (upButton.getGlobalBounds().contains(mousePos) && first > 0)
		{
			first--;
			last--;

			for (BulletPatternButton& button : buttons)
				button.moveUp();
		}
		else if (downButton.getGlobalBounds().contains(mousePos) &&
			last < buttons.size()-1)
		{
			first++;
			last++;

			for (BulletPatternButton& button : buttons)
				button.moveDown();
		}
	}
	

	for (unsigned int i = first; i <= last && i < buttons.size(); i++)
		buttons[buttonOrder[i]].update(window, frame, enemies, selectedEnemyIndex, textureMap, currentFrame);

	window.draw(menuRect);
	
	if (buttons.size() > 9)
	{
		window.draw(upButton);
		window.draw(upIcon);

		window.draw(downButton);
		window.draw(downIcon);
	}
}