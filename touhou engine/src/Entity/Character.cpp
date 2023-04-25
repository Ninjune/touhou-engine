// base class for entities
#include <iostream>
#include "Character.h"


Character::Character()
{
	render = true;
	spriteLeft = 0, spriteTop = 0, spriteWidth = 0, spriteHeight = 0;
	hitboxMod = 1.f;
}


sf::FloatRect Character::getHitbox()
{
	sf::FloatRect spriteBounds = sprite.getGlobalBounds();
	return sf::FloatRect(spriteBounds.left, spriteBounds.top, 
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