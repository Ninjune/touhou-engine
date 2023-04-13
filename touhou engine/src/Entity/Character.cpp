// base class for entities
#include <iostream>
#include "Character.h"


Character::Character()
{
	render = true;
	spriteLeft = 0, spriteTop = 0, spriteWidth = 0, spriteHeight = 0;
}


sf::FloatRect Character::getHitbox()
{
	return sprite.getGlobalBounds();
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