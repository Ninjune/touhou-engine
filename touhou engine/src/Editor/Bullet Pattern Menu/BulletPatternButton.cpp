#include "BulletPatternButton.h"

BulletPatternButton::BulletPatternButton(sf::Vector2f menuPos, sf::Vector2f menuSize,
	std::vector<BulletPatternButton> buttons, BulletPattern pattern)
{
	mainRectangle.setPosition(menuPos.x, menuPos.y + 30 * buttons.size() - 1);
	mainRectangle.setSize(sf::Vector2f(menuSize.x, 30));

	innerLine.setPosition(mainRectangle.getPosition().x + menuSize.x/6*5, 
		mainRectangle.getPosition().y);
	innerLine.setSize(sf::Vector2f(1, menuSize.y));
}


void BulletPatternButton::update(sf::RenderWindow& window)
{
	window.draw(mainRectangle);
	window.draw(innerLine);
}