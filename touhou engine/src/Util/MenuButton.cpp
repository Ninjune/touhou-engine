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


void MenuButton::draw(sf::RenderWindow& window)
{
	window.draw(*this);
	window.draw(sprite);
}