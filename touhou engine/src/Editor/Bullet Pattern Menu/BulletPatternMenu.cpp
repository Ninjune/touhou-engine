#include "BulletPatternMenu.h"


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

	addPatternButton.setSize(sf::Vector2f(32, 32));
	addPatternButton.setOutlineThickness(1);

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
	addPatternButton.setPosition(menuRect.getPosition().x,
		menuRect.getPosition().y + menuRect.getSize().y
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

	addPatternButton.setupSprite(textureMap["plusIcon"], 512, 512);
	pFont = &font;

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
				*pFont
			));

		buttonOrder.push_back(count++);
	}

	sort();
}

void BulletPatternMenu::update(sf::RenderWindow& window,
	int& frame,
	std::vector<Enemy>& enemies,
	int& selectedEnemyIndex,
	std::map<std::string, sf::Texture>& textureMap,
	int currentFrame,
	std::string& patternChanging,
	std::map<std::string, BulletPattern>& patterns,
	bool& addingPattern
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
			last < buttons.size() - 1)
		{
			first++;
			last++;

			for (BulletPatternButton& button : buttons)
				button.moveDown();
		}
		else if (addPatternButton.checkMouse(window))
		{
			// add pattern editor
			addingPattern = true;
		}
	}

	for (unsigned int i = first; i <= last && i < buttons.size(); i++)
		buttons[buttonOrder[i]].update(window,
			frame,
			enemies,
			selectedEnemyIndex,
			textureMap,
			currentFrame,
			patternChanging,
			buttons,
			patterns
		);

	window.draw(menuRect);
	addPatternButton.draw(window);
	//patternChanger = addPatternButton.getState();

	if (buttons.size() > 9)
	{
		window.draw(upButton);
		window.draw(upIcon);

		window.draw(downButton);
		window.draw(downIcon);
	}
}


void BulletPatternMenu::erase(std::string buttonName)
{
	int index = 0;

	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].getName() == buttonName)
		{
			buttons.erase(buttons.begin() + i);
			while (buttonOrder[index] != i && index < buttonOrder.size())
				index++;
			buttonOrder.erase(buttonOrder.begin() + index);
			for (unsigned int j = 0; j < buttonOrder.size(); j++)
				if (buttonOrder[j] >= index)
					buttonOrder[j] -= 1;

			for (unsigned int j = i; j < buttons.size(); j++)
				buttons[j].moveDown();
		}
	}
}


void BulletPatternMenu::addButton(BulletPattern& pattern)
{
	buttons.push_back(BulletPatternButton(menuRect.getPosition(),
		menuRect.getSize(),
		buttons,
		pattern,
		*pFont)
	);
	buttonOrder.push_back(buttons.size() - 1);
}


void BulletPatternMenu::sort()
{
	unsigned int temp;
	if (buttons.size() <= 0)
		return;

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