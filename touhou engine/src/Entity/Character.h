#include <SFML/Graphics.hpp>
#pragma once
class Character
{
public:
	Character();
	void setRender(bool);
	bool getRender();
	// hitboxSize *= modifier
	void modifyHitbox(float modifier);
	// life += modifier
	void changeLife(int modifier);
	sf::Vector2f getPosition();
	sf::FloatRect getHitbox();
	void renderHitbox(sf::RenderWindow& window);
protected:
	sf::Sprite sprite;
	int spriteLeft, spriteTop, spriteWidth, spriteHeight;
	// you will need to manually immunity-- in whatever entity is using it's update.
	int life, immunity, immunityMax;
private:
	bool render;
	float hitboxMod;
};

