#include <SFML/Graphics.hpp>
#include <fstream>
#include <filesystem>
#include "StageLoaderButton.h"
#include "../Entity/Enemy.h"
#pragma once

class StageLoader
{
public:
	StageLoader();
	void update(sf::RenderWindow& window,
		int& frame,
		std::map<std::string, sf::Texture>& textureMap,
		sf::Font& font,
		std::string& state,
		std::vector<Enemy>& enemies,
		int& stageLength
	);
private:
	sf::RectangleShape menuRect;
	std::vector<StageLoaderButton> buttons;
	std::vector<std::string> stages;
};