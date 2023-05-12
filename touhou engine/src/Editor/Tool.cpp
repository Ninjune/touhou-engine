#include "Tool.h"

Tool::Tool(sf::Texture& in, float x, float y, int size, int spriteSize, bool in2)
	: m1(sf::Mouse::Left)
{
	setSize(sf::Vector2f(size, size));
	setPosition(x, y);
	setOutlineThickness(1);

	setupSprite(in, spriteSize, spriteSize);
	setState(in2);
}


void Tool::update(sf::RenderWindow& window,
	int& frame,
	std::vector<Tool>& tools
)
{
	const sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x,
		sf::Mouse::getPosition(window).y);
	if (getGlobalBounds().contains(mousePos) && m1.consumeClick(frame, 20))
	{
		for (Tool& tool : tools)
			tool.disable();
		setState(true);
	}

	if (getState())
		setFillColor(sf::Color(39, 170, 245, 43));
	else
		setFillColor(sf::Color::Transparent);

	draw(window);
}


bool Tool::getStatus()
{
	return getState();
}


void Tool::disable()
{
	setState(false);
}


