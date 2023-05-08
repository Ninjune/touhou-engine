#include <SFML/Graphics.hpp>
#include <fstream>
#include <filesystem>
#include "StageLoaderButton.h"
#pragma once

class StageLoader
{
public:
	StageLoader();
	void update(sf::RenderWindow& window,
		int& frame,
		sf::Font& font
	);
private:
	sf::RectangleShape menuRect;
	std::vector<StageLoaderButton> buttons;
};