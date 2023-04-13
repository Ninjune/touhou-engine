#include <SFML/Graphics.hpp>
#include <thread>
#include <map>
#pragma once
class Keybind
{
public:
	Keybind(sf::Keyboard::Key);
	Keybind(sf::Mouse::Button);
	bool consumeClick(int& frame, int framesToWait, int id = 0);
	bool isKey();
	bool isHeld();
private:
	sf::Keyboard::Key key;
	sf::Mouse::Button button;
	bool keyBool;
	std::map<int, int> cooldowns; // id to ...
	std::map<int, int> initialFrames;
};

