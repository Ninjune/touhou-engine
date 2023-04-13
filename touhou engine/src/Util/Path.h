#include <SFML/Graphics.hpp>
#pragma once
class Path : public std::vector<sf::Vector2f>
{
public:
	using std::vector<sf::Vector2f>::vector;
	void setPathSpeed(int);
	int getPathSpeed();
	bool smoothPath();
private:
	int pathSpeed = 1;
};

