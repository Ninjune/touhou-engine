#include "BulletPatternButton.h"


BulletPatternButton::BulletPatternButton(sf::Vector2f menuPos, sf::Vector2f menuSize,
	std::vector<BulletPatternButton> buttons, BulletPattern pattern,
	sf::Texture& mTexture, sf::Texture& pTexture) :
	bulletPattern(pattern)
{
	mainRectangle.setPosition(menuPos.x, menuPos.y + 30 * buttons.size() - 1);
	mainRectangle.setSize(sf::Vector2f(menuSize.x, menuSize.y/9));
	mainRectangle.setFillColor(sf::Color::Transparent);
	mainRectangle.setOutlineColor(sf::Color::White);
	mainRectangle.setOutlineThickness(1);

	innerRectangle.setPosition(mainRectangle.getPosition().x + menuSize.x/6*5, 
		mainRectangle.getPosition().y);
	innerRectangle.setSize(sf::Vector2f(menuSize.x/6, mainRectangle.getSize().y));
	innerRectangle.setFillColor(sf::Color::Transparent);
	innerRectangle.setOutlineColor(sf::Color::White);
	innerRectangle.setOutlineThickness(1);

	addedIndex = -1;
	plusTexture = pTexture;
	minusTexture = mTexture;

	buttonSprite.setPosition(innerRectangle.getPosition().x, innerRectangle.getPosition().y);
	buttonSprite.setScale(32. / 512, 32. / 512);
}


void BulletPatternButton::update(sf::RenderWindow& window, std::vector<Enemy>& enemies, int selectedEnemyIndex)
{
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
	if (innerRectangle.getGlobalBounds().contains(mousePos))
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
	{
		buttonSprite.setTexture(plusTexture);
	}
	else
	{
		buttonSprite.setTexture(minusTexture);
	}

	window.draw(buttonSprite);
	window.draw(mainRectangle);
	window.draw(innerRectangle);
}