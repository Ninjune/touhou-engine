#include "BulletPatternButton.h"


BulletPatternButton::BulletPatternButton(sf::Vector2f menuPos,
	sf::Vector2f menuSize,
	std::vector<BulletPatternButton> buttons,
	BulletPattern& pattern,
	sf::Font& font
) : m1(sf::Mouse::Left)
{
	mainRectangle.setPosition(menuPos.x, menuPos.y + 32 * buttons.size() - 1);
	mainRectangle.setSize(sf::Vector2f(menuSize.x, 32));
	mainRectangle.setFillColor(sf::Color::Transparent);
	mainRectangle.setOutlineColor(sf::Color::White);
	mainRectangle.setOutlineThickness(1);
	
	patternName.setPosition(mainRectangle.getPosition().x, 
		mainRectangle.getPosition().y
	);
	patternName.setCharacterSize(14);
	patternName.setString(pattern.getName());
	patternName.setFont(font);

	play.setPosition(mainRectangle.getPosition().x + menuSize.x/6*5, 
		mainRectangle.getPosition().y);
	play.setSize(sf::Vector2f(menuSize.x/6, mainRectangle.getSize().y));
	play.setOutlineThickness(1);

	edit.setPosition(play.getPosition().x - menuSize.x/6, mainRectangle.getPosition().y);
	edit.setSize(sf::Vector2f(menuSize.x / 6, mainRectangle.getSize().y));
	edit.setOutlineThickness(1);
}


void BulletPatternButton::update(sf::RenderWindow& window,
	int& frame,
	std::vector<Enemy>& enemies,
	int& selectedEnemyIndex,
	std::map<std::string, sf::Texture>& textureMap,
	int currentFrame,
	std::string& patternChanging,
	std::vector<BulletPatternButton>& buttons,
	std::map<std::string, BulletPattern>& patterns
) 
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, 
		sf::Mouse::getPosition(window).y
	);
	std::vector<std::string> patternCopy = enemies[selectedEnemyIndex].getPatterns();
	std::vector<int> startCopy = enemies[selectedEnemyIndex].getStartTimes();
	play.setState(false);

	for (unsigned int i = 0; i < startCopy.size(); i++)
		if (startCopy[i] == currentFrame &&
			patternCopy[i] == getName()
		)
			play.setState(true);
	
	if (patternChanging == "" && play.checkMouse(window) && m1.consumeClick(frame, 20))
	{
		if (!play.getState() && enemies[selectedEnemyIndex].getRender())
		{
			enemies[selectedEnemyIndex]
				.pushToPatterns(patterns[patternName.getString()], currentFrame);
			play.setState(true);
		}
		else
		{
			enemies[selectedEnemyIndex].eraseFromPatterns(currentFrame);
			play.setState(false);
		}
	}

	if (edit.checkMouse(window) && m1.consumeClick(frame, 20, 1))
	{
		bool temp;

		temp = edit.getState();
		for (BulletPatternButton& button : buttons)
			button.disableEdit();
		edit.setState(!temp);

		if (edit.getState())
			patternChanging = getName();
		else
			patternChanging = "";
	}

	if (!play.getState())
		play.setupSprite(textureMap["plusIcon"], 512, 512);
	else
		play.setupSprite(textureMap["minusIcon"], 512, 512);
	edit.setupSprite(textureMap["pencilIcon"], 512, 512);

	if (!edit.getState())
		edit.setFillColor(sf::Color::Transparent);
	else
		edit.setFillColor(sf::Color(39, 170, 245, 73));
	
	window.draw(patternName);
	window.draw(mainRectangle);
	edit.draw(window);
	play.draw(window);
}


std::string BulletPatternButton::getName()
{
	return patternName.getString();
}


bool BulletPatternButton::getPlayStatus()
{
	return play.getState();
}


void BulletPatternButton::disableEdit()
{
	edit.setState(false);
}


void BulletPatternButton::moveUp()
{
	mainRectangle.move(0, 32);
	patternName.move(0, 32);

	play.move(0, 32);
	edit.move(0, 32);
}


void BulletPatternButton::moveDown()
{
	mainRectangle.move(0, -32);
	patternName.move(0, -32);

	play.move(0, -32);
	edit.move(0, -32);
}