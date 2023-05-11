#include "../../Util/MenuButton.h"
#include "../../Util/Keybind.h"
#pragma once


class StageLoaderButton
{
public:
	StageLoaderButton(std::map<std::string, sf::Texture>& textureMap,
		sf::Vector2f menuSize,
		sf::Vector2f menuPos,
		std::string stageIn,
		sf::Font& font,
		std::vector<StageLoaderButton> buttons
	);
	void update(sf::RenderWindow& window, int& frame);
	std::string getState();
	std::string getStageName();
	void setState(std::string in);
private:
	sf::RectangleShape mainRect;
	MenuButton edit, play;
	sf::Text stageName;
	std::string stage;
	Keybind m1;
	std::string state;
};

