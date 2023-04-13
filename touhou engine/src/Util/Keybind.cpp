#include "Keybind.h"


Keybind::Keybind(sf::Keyboard::Key in)
{
	waitFrame = 0;
	key = in;
	keyBool = true;
}


Keybind::Keybind(sf::Mouse::Button in)
{
	waitFrame = 0;
	button = in;
	keyBool = false;
}


bool Keybind::consumeClick(int& frame, int framesToWait)
{
	if (waitFrame == 0 && (isKey() ? sf::Keyboard::isKeyPressed(key) : sf::Mouse::isButtonPressed(button)))
	{
		waitFrame = framesToWait;
		return true;
	}
	else if (waitFrame > 0)
		waitFrame--;

	return false;
}


bool Keybind::isKey()
{
	return keyBool;
}