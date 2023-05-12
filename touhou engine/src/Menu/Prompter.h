#include <SFML/Graphics.hpp>
#include "../Util/Keybind.h"
#pragma once


// used to reuse prompt across files.
class Prompter : public sf::Text
{
public:
	Prompter();
	void prompt(sf::RenderWindow& window, std::string input, int& frame);
	// inclusive both side
	void poll(sf::Event& event,
		unsigned int minUnicode,
		unsigned int maxUnicode,
		std::string& inputStr,
		std::vector<char> exceptions = {},
		std::vector<char> includes = {},
		bool useCursor = false
	);
	 int getCursorIndex();
	 bool search(std::vector<char> arr, char val);
private:
	sf::RectangleShape cursor;
	int cursorIndex;
	Keybind m1, left, right;
};