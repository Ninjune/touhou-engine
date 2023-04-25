#include <SFML/Graphics.hpp>
#pragma once
class Character
{
public:
	Character();
	void setRender(bool);
	bool getRender();
	void modifyHitbox(float modifier); // hitboxSize *= modifier
	sf::Vector2f getPosition();
	sf::FloatRect getHitbox();
protected:
	sf::Sprite sprite;
	int spriteLeft, spriteTop, spriteWidth, spriteHeight;
private:
	bool render;
	float hitboxMod;
};

