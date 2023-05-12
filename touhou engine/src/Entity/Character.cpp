// base class for entities
#include <iostream>
#include "Character.h"


Character::Character()
{
	render = true;
	spriteLeft = 0, spriteTop = 0, spriteWidth = 0, spriteHeight = 0;
	hitboxMod = 1.f;
	immunity = 0;
	immunityMax = 120;
	life = 1;
}


sf::FloatRect Character::getHitbox()
{
	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	return sf::FloatRect(spriteBounds.left + spriteBounds.width*((1-hitboxMod)/2),
		spriteBounds.top + spriteBounds.height*((1-hitboxMod)/2),
		spriteBounds.width * hitboxMod, spriteBounds.height * hitboxMod);
}


void Character::modifyHitbox(float modifier)
{
	hitboxMod = modifier;
}


void Character::setRender(bool in)
{
	render = in;
}


bool Character::getRender()
{
	return render;
}


sf::Vector2f Character::getPosition()
{
	return sprite.getPosition();
}


void Character::changeLife(int mod)
{
	life += mod;
}


void Character::renderHitbox(sf::RenderWindow& window) // for debug & selcet
{
	sf::RectangleShape rect;
	sf::FloatRect hb = getHitbox();

	rect.setPosition(hb.left, hb.top);
	rect.setSize(sf::Vector2f(hb.width, hb.height));
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setOutlineColor(sf::Color::White);

	window.draw(rect);
}