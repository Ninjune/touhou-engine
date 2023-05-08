#include "MenuButton.h"

MenuButton::MenuButton()
{
	state = false;
	setFillColor(sf::Color::Transparent);
	setOutlineColor(sf::Color::White);
}


void MenuButton::setState(bool in)
{
	state = in;
}


// set position & size prior to this.
void MenuButton::setupSprite(sf::Texture& texture, int sizeX, int sizeY)
{
	sprite.setTexture(texture);
	sprite.setPosition(getPosition().x, getPosition().y);
	sprite.setScale((float)getSize().x / sizeX, (float)getSize().y / sizeY);
}


bool MenuButton::getState()
{
	return state;
}


bool MenuButton::checkMouse()
{
	if (getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition().x,
		sf::Mouse::getPosition().y))
	)
		return true;
	return false;
}


void MenuButton::draw(sf::RenderWindow& window)
{
	window.draw(*this);
	window.draw(sprite);
}