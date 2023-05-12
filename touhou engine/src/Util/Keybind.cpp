#include "Keybind.h"


Keybind::Keybind(sf::Keyboard::Key in)
{
	key = in;
	keyBool = true;
}


Keybind::Keybind(sf::Mouse::Button in)
{
	button = in;
	keyBool = false;
}


// note: consume click last in && sequences.
bool Keybind::consumeClick(int& frame, int framesToWait, int id)
{
	if (!initialFrames.count(id))
	{
		initialFrames[id] = frame;
		cooldowns[id] = framesToWait;
		return false;
	}
	else if ((frame-initialFrames[id] >= cooldowns[id]) &&
		(isKey() ? sf::Keyboard::isKeyPressed(key) : sf::Mouse::isButtonPressed(button)))
	{
		initialFrames[id] = frame;
		cooldowns[id] = framesToWait;
		return true;
	}

	return false;
}


bool Keybind::isHeld()
{
	if (isKey() ? sf::Keyboard::isKeyPressed(key) : sf::Mouse::isButtonPressed(button))
		return true;
	return false;
}


bool Keybind::isKey()
{
	return keyBool;
}