#include <SFML/Graphics.hpp>
#include "../Util/Keybind.h"
#pragma once


class Tool // options texture, x, y, size
{
public:
	Tool(sf::Texture& texture, float x, float y, int size, int spriteSize, bool enabled = false);
	void update(sf::RenderWindow& window, int& frame, std::vector<Tool>& tools); // pass it tools so we can disable other tools when this one is enabled
	bool getStatus();
	void disable();
private:
	Keybind m1;
	sf::Sprite sprite;
	sf::RectangleShape box;
	bool enabled;
};

