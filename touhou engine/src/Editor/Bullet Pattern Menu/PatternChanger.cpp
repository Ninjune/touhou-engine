#include "PatternChanger.h"


PatternChanger::PatternChanger()
	: m1(sf::Mouse::Left)
{
	del.setSize(sf::Vector2f(32, 32));
	del.setOutlineThickness(1);
}


void PatternChanger::update(sf::RenderWindow& window,
	int& frame,
	BulletPatternMenu& menu,
	std::map<std::string, BulletPattern>& patterns,
	std::map<std::string, sf::Texture>& textures,
	std::string& patternChanging,
	BulletPatternMenu& patternMenu
)
{
	sf::FloatRect textRect;
	BulletPattern& pattern = patterns[patternChanging];
	std::map<std::string, float> options = pattern.getOptions();

	valuesStr.clear();
	keysStr.clear();

	for (std::pair<std::string, float> option : options)
	{
		keysStr += option.first + ": \n";
		valuesStr += std::to_string(option.second) + "\n";
	}

	prompter.prompt(window, valuesStr, frame);

	keys.setString(keysStr);
	textRect = keys.getLocalBounds();
	keys.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f
	);
	keys.setPosition(window.getSize().x / 4, window.getSize().y / 2);
	window.draw(keys);

	if (del.checkMouse(window) && m1.consumeClick(frame, 20))
	{
		patterns.erase(patterns.find(patternChanging));
		patternMenu.erase(patternChanging);
		std::filesystem::remove("patterns/" + patternChanging + ".txt");
		patternChanging = "";
	} // dont use pattern onward

	del.setPosition(window.getSize().x / 2, window.getSize().y / 6*5);
	del.setupSprite(textures["trashIcon"], 512, 512);
	del.draw(window);
}


void PatternChanger::poll(sf::Event& event, BulletPattern& pattern)
{
	if (keysStr.size() <= 0)
		return;
	std::map<std::string, float> options;
	int lineCount = 0;
	prompter.poll(event, 48, 57, valuesStr, {}, { '.', '-'}, true);
	for (int i = 0; i < valuesStr.size(); i++)
		if (valuesStr[i] == '\n')
			lineCount++;

	for (int i = 0; i < lineCount; i++)
		options[getKey(i)] = stod(getValue(i));

	pattern.setOptions(options);
}


void PatternChanger::setFont(sf::Font& fontIn)
{
	prompter.setFont(fontIn);
	keys.setFont(fontIn);
}


std::string PatternChanger::getKey(int index)
{
	int count = index;
	std::string out;
	int i = 0;

	while (count != 0)
	{
		if (keysStr[i++] == '\n')
			count--;
	}

	while (keysStr[i] != ':')
		out += keysStr[i++];

	return out;
}


std::string PatternChanger::getValue(int index)
{
	int count = index;
	std::string out;
	int i = 0;

	while (count != 0)
	{
		if (valuesStr[i++] == '\n')
			count--;
	}

	while (valuesStr[i] != '\n')
		out += valuesStr[i++];

	return out;
}