#include "StageLoaderButton.h"


StageLoaderButton::StageLoaderButton(std::map<std::string, sf::Texture>& textureMap,
	sf::Vector2u menuSize,
	std::string stageIn,
	sf::Font& font
) : m1(sf::Mouse::Left)
{
	stage = stageIn;

	mainRect.setSize(sf::Vector2f(menuSize.x, 32));
	mainRect.setOutlineColor(sf::Color::White);
	mainRect.setFillColor(sf::Color::Transparent);
	mainRect.setOutlineThickness(1);

	play.setPosition(mainRect.getPosition().x + mainRect.getSize().x-mainRect.getSize().y,
		mainRect.getPosition().y);
	play.setSize(sf::Vector2f(32, 32.f));
	play.setupSprite(textureMap["plusIcon"], 512, 512);

	edit.setPosition(play.getPosition().x - 32, play.getPosition().y);
	edit.setSize(sf::Vector2f(play.getSize().x, play.getSize().y));
	edit.setupSprite(textureMap["pencilIcon"], 512, 512);

	stageName.setPosition(mainRect.getPosition().x,
		mainRect.getPosition().y + mainRect.getSize().y/2
	);
	stageName.setCharacterSize(14);
	stageName.setString(stage);
	stageName.setFont(font);

	state = "loader";
}


void StageLoaderButton::update(sf::RenderWindow& window, int& frame)
{
	if (play.checkMouse() && m1.consumeClick(frame, 20))
		state = "play";
	else if (edit.checkMouse() && m1.consumeClick(frame, 20))
		state = "edit";

	window.draw(mainRect);
	window.draw(stageName);
	play.draw(window);
	edit.draw(window);
}


std::string StageLoaderButton::getState()
{
	return state;
}


std::string StageLoaderButton::getStageName()
{
	return stage;
}