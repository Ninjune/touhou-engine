#include "Timeline.h"

Timeline::Timeline(const int windowWidth, const int windowHeight,
	sf::Font& font)
	: left(sf::Keyboard::Left), right(sf::Keyboard::Right),
	m1(sf::Mouse::Left), spacebar(sf::Keyboard::Space)
{
	outerRectangle.setFillColor(sf::Color::Transparent);
	outerRectangle.setOutlineColor(sf::Color::White);
	outerRectangle.setOutlineThickness(2);
	outerRectangle.setSize(sf::Vector2f(windowWidth/8 * 6, windowHeight/12));
	outerRectangle.setOrigin(outerRectangle.getSize().x / 2, 
		outerRectangle.getSize().y / 2);
	outerRectangle.setPosition(windowWidth/2, windowHeight/18*17);

	innerRectangle.setFillColor(sf::Color::Red);
	innerRectangle.setPosition(outerRectangle.getPosition().x - outerRectangle.getSize().x / 2,
		outerRectangle.getPosition().y - outerRectangle.getSize().y / 2);
	currentFrame = 0;
	stageLength = 1;
	frameText.setFont(font);

	playButton.setOutlineThickness(2);
	playButton.setSize(sf::Vector2f(outerRectangle.getSize().y, outerRectangle.getSize().y));
	playButton.setOrigin(playButton.getSize().x / 2, playButton.getSize().y / 2);
	playButton.setPosition(windowWidth / 11, outerRectangle.getPosition().y);

	playTriangle.setRadius(playButton.getSize().x/3);
	playTriangle.setOrigin(playTriangle.getRadius(), playTriangle.getRadius());
	playTriangle.setRotation(90);
	playTriangle.setPointCount(3);
	playTriangle.setFillColor(sf::Color::Red);
	playTriangle.setOutlineColor(sf::Color::White);
	playTriangle.setOutlineThickness(0.5);
	playTriangle.setPosition(playButton.getPosition());
}


int Timeline::getCurrentFrame()
{
	return currentFrame;
}


void Timeline::update(sf::RenderWindow& window, int& frame)
{
	bool run = true;
	sf::FloatRect tempRect;
	int low = 0, high = bounds.size()-1, middle;
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x,
		sf::Mouse::getPosition(window).y);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (outerRectangle.getGlobalBounds().contains(mousePos))
		{
			while (run)
			{
				middle = (low + high) / 2;
				if (bounds[middle].contains(mousePos))
				{
					tempRect = bounds[middle];
					currentFrame = middle;
					run = false;
					break;
				}
				else if (bounds[middle].left < mousePos.x)
					low = middle + 1;
				else if (bounds[middle].left > mousePos.x)
					high = middle - 1;
				run = low <= high;
			}
		}
	}

	if ((m1.consumeClick(frame, 20) &&
		playButton.getGlobalBounds().contains(mousePos)) ||
		spacebar.consumeClick(frame, 20))
	{
		playButton.setState(!playButton.getState());
		if (currentFrame >= stageLength - 1)
			currentFrame = 0;
	}

	if (playButton.getState())
	{
		playTriangle.setFillColor(sf::Color::Green);
		if (currentFrame < stageLength - 1)
			currentFrame++;
		else
			playButton.setState(false);
	}
	else
		playTriangle.setFillColor(sf::Color::Red);

	

	if (left.consumeClick(frame, 5))
	{
		playButton.setState(false);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			currentFrame -= 60;
		else
			currentFrame -= 1;
	}

	if (right.consumeClick(frame, 5))
	{
		playButton.setState(false);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			currentFrame += 60;
		else if (currentFrame + 1 < stageLength)
			currentFrame += 1;
	}

	if (currentFrame >= stageLength)
		currentFrame = stageLength - 1;
	else if (currentFrame < 0)
		currentFrame = 0;

	innerRectangle.setPosition(bounds[currentFrame].left, bounds[currentFrame].top);
	frameText.setString(std::to_string(currentFrame));
	frameText.setPosition(innerRectangle.getPosition().x,outerRectangle.getPosition().y);
	window.draw(outerRectangle);
	window.draw(innerRectangle);
	window.draw(playButton);
	window.draw(playTriangle);
	window.draw(frameText);
}


void Timeline::setStageLength(int in)
{
	stageLength = in;
	bounds.clear();
	sf::FloatRect tempRect;
	for (int i = 0; i < stageLength; i++)
	{
		tempRect.height = innerRectangle.getSize().y;
		tempRect.width = innerRectangle.getSize().x;
		tempRect.left = (outerRectangle.getPosition().x -
			outerRectangle.getSize().x / 2) +
			outerRectangle.getSize().x / stageLength * i;
		tempRect.top = outerRectangle.getPosition().y - outerRectangle.getSize().y / 2;
		bounds.push_back(tempRect);
	}
	innerRectangle.setSize(sf::Vector2f(ceil(outerRectangle.getSize().x / stageLength),
		outerRectangle.getSize().y));
}