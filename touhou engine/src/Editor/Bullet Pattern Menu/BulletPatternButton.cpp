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
	mainRectangle.setPosition(menuPos.x, menuPos.y + 30 * buttons.size() - 1);
	mainRectangle.setSize(sf::Vector2f(menuSize.x, menuSize.y/9));
	mainRectangle.setFillColor(sf::Color::Transparent);
	mainRectangle.setOutlineColor(sf::Color::White);
	mainRectangle.setOutlineThickness(1);
	
	patternName.setPosition(mainRectangle.getPosition().x, mainRectangle.getPosition().y);
	patternName.setCharacterSize(14);
	patternName.setString(pattern.getName());
	patternName.setFont(font);

	innerRectangle.setPosition(mainRectangle.getPosition().x + menuSize.x/6*5, 
		mainRectangle.getPosition().y);
	innerRectangle.setSize(sf::Vector2f(menuSize.x/6, mainRectangle.getSize().y));
	innerRectangle.setFillColor(sf::Color::Transparent);
	innerRectangle.setOutlineColor(sf::Color::White);
	innerRectangle.setOutlineThickness(1);

	addedIndex = -1;

	buttonSprite.setPosition(innerRectangle.getPosition().x, innerRectangle.getPosition().y);
	buttonSprite.setScale(32. / 512, 32. / 512);
}


void BulletPatternButton::update(sf::RenderWindow& window,
	int& frame,
	std::vector<Enemy>& enemies,
	int selectedEnemyIndex,
	std::map<std::string, sf::Texture>& textureMap
) 
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	if (innerRectangle.getGlobalBounds().contains(mousePos)  && m1.consumeClick(frame, 20))
	{
		if (addedIndex < 0)
			addedIndex = enemies[selectedEnemyIndex].pushToPatterns(bulletPattern);
		else
		{
			enemies[selectedEnemyIndex].eraseFromPatterns(addedIndex);
			addedIndex = -1;
		}
	}

	if (addedIndex < 0)
		buttonSprite.setTexture(textureMap["plusIcon"]);
	else
		buttonSprite.setTexture(textureMap["minusIcon"]);

	window.draw(patternName);
	window.draw(mainRectangle);
	window.draw(innerRectangle);
	window.draw(buttonSprite);
}