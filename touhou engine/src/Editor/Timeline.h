#include <SFML/Graphics.hpp>
#include <vector>
#include "../Util/Keybind.h"

#pragma once
class Timeline
{
	// 1 transparent rectangle with white stroke with another vertical rectangle inside it with red fill
public:
	Timeline(const int, const int, sf::Font&);
	void update(sf::RenderWindow&, int& frame);
	void setStageLength(int);
	int getCurrentFrame();
private:
	Keybind left, right, m1, spacebar;
	sf::RectangleShape outerRectangle, innerRectangle, playRectangle;
	sf::CircleShape playTriangle;
	bool play;
	int currentFrame, stageLength;
	std::vector<sf::FloatRect> bounds;
	sf::Text frameText;
};

