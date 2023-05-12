#include <SFML/Graphics.hpp>
#include <filesystem>
#include "BulletPatternMenu.h"
#include "../../Menu/Prompter.h"
#pragma once

class PatternChanger
{
public:
	PatternChanger();
	void update(sf::RenderWindow& window,
		int& frame,
		BulletPatternMenu& menu,
		std::map<std::string, BulletPattern>& patterns,
		std::map<std::string, sf::Texture>& textures,
		std::string& patternChanging,
		BulletPatternMenu& patternMenu
	);
	void poll(sf::Event& event, BulletPattern& pattern);
	void setFont(sf::Font& font);
private:
	Prompter prompter;
	std::string valuesStr;
	std::string keysStr;
	sf::Text keys;
	MenuButton del;
	Keybind m1;

	std::string getKey(int index);
	std::string getValue(int index);
};

