#include <SFML/Graphics.hpp>
#include <thread>
#pragma once
class Keybind
{
public:
	Keybind(sf::Keyboard::Key);
	Keybind(sf::Mouse::Button);
	bool consumeClick(int& frame, int framesToWait);
	bool isKey();
private:
	sf::Keyboard::Key key;
	sf::Mouse::Button button;
	bool keyBool;
	int waitFrame;
};

