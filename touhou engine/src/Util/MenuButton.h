#include <SFML/Graphics.hpp>
#pragma once

/*
Used to generalize some parts of buttons.
*/
class MenuButton : public sf::RectangleShape
{
public:
	MenuButton();
	void setState(bool in);
	// setups the sprite, call after setting position of the button.
	void setupSprite(sf::Texture& texture, int sizeX, int sizeY);
	bool getState();
	void draw(sf::RenderWindow& window);
protected:
	sf::Sprite sprite;
private:
	bool state;
};
