
#include "BulletPatternButton.h"


BulletPatternButton::BulletPatternButton(sf::Vector2f menuPos,
	sf::Vector2f menuSize,
	std::vector<BulletPatternButton> buttons,
	BulletPattern pattern,
	sf::Font& font
) :
	bulletPattern(pattern),
	m1(sf::Mouse::Left)
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

	setPosition(mainRectangle.getPosition().x + menuSize.x/6*5, 
		mainRectangle.getPosition().y);
	setSize(sf::Vector2f(menuSize.x/6, mainRectangle.getSize().y));
	setOutlineThickness(1);
}


void BulletPatternButton::update(sf::RenderWindow& window,
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
	std::vector<std::string> patternCopy = enemies[selectedEnemyIndex].getPatterns();
	std::vector<int> startCopy = enemies[selectedEnemyIndex].getStartTimes();
	setState(false);

	for (unsigned int i = 0; i < startCopy.size(); i++)
		if (startCopy[i] == currentFrame &&
			patternCopy[i] == getName()
		)
			setState(true);
	
	if (getGlobalBounds().contains(mousePos) &&
		m1.consumeClick(frame, 20)
	)
	{
		if (!getState() && enemies[selectedEnemyIndex].getRender())
		{
			enemies[selectedEnemyIndex].pushToPatterns(bulletPattern, currentFrame);
			setState(true);
		}
		else
		{
			enemies[selectedEnemyIndex].eraseFromPatterns(currentFrame);
			setState(false);
		}
	}

	if (!getState())
		setupSprite(textureMap["plusIcon"], 512, 512);
	else
		setupSprite(textureMap["minusIcon"], 512, 512);

	window.draw(patternName);
	window.draw(mainRectangle);
	draw(window);
}


std::string BulletPatternButton::getName()
{
	return patternName.getString();
}


bool BulletPatternButton::getStatus()
{
	return getState();
}

void BulletPatternButton::moveUp()
{
	mainRectangle.move(0, 32);
	patternName.move(0, 32);
	move(0, 32);
	sprite.move(0, 32);
}


void BulletPatternButton::moveDown()
{
	mainRectangle.move(0, -32);
	patternName.move(0, -32);
	move(0, -32);
	sprite.move(0, -32);
}