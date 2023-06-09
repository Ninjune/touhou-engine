#include <SFML/Graphics.hpp>
#include <vector>
#include "../Util/Keybind.h"
#include "../Util/MenuButton.h"

#pragma once
class Timeline
{
public:
	Timeline(const int, const int, sf::Font&);
	void update(sf::RenderWindow&, int& frame);
	void setStageLength(int);
	int getCurrentFrame();
private:
	Keybind left, right, m1, spacebar;
	sf::RectangleShape outerRectangle, innerRectangle;
	MenuButton playButton;
	sf::CircleShape playTriangle;
	int currentFrame, stageLength;
	std::vector<sf::FloatRect> bounds;
	sf::Text frameText;
};

