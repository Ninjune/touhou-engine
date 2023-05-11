#include <SFML/Graphics.hpp>
#include "BulletPatternMenu.h"
#pragma once

class PatternChanger
{
public:
	PatternChanger();
	void update(sf::RenderWindow& window,
		int& frame,
		BulletPatternMenu& menu,
		std::map<std::string, BulletPattern>& patterns
	);
	void poll(sf::Event event);
private:
	sf::RectangleShape cursor;
	sf::Text text;
	std::string textStr;
	unsigned int cursorIndex;
};

