#include "Tool.h"

Tool::Tool(sf::Texture& in, float x, float y, int size, int spriteSize, bool in2)
	: m1(sf::Mouse::Left)
{
	box.setSize(sf::Vector2f(size, size));
	box.setOrigin(size/2, size/2);
	box.setPosition(x, y);
	box.setFillColor(sf::Color::Transparent);
	box.setOutlineColor(sf::Color::White);
	box.setOutlineThickness(1);

	sprite.setTexture(in);
	sprite.setOrigin(spriteSize/2, spriteSize/2);
	sprite.setPosition(x, y);
	sprite.setScale((float)size/spriteSize, (float)size/spriteSize);
	enabled = in2;
}


void Tool::update(sf::RenderWindow& window, int& frame, std::vector<Tool>& tools)
{
	const sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	if (box.getGlobalBounds().contains(mousePos) && m1.consumeClick(frame, 20))
	{
		for (Tool& tool : tools)
			tool.disable();
		enabled = true;
	}

	if (enabled)
		box.setFillColor(sf::Color(39, 170, 245, 43));
	else
		box.setFillColor(sf::Color::Transparent);

	window.draw(sprite);
	window.draw(box);
}


bool Tool::getStatus()
{
	return enabled;
}


void Tool::disable()
{
	enabled = false;
}


