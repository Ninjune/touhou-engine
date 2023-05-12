#include "StageLoader.h"


StageLoader::StageLoader()
	: m1(sf::Mouse::Left)
{
	menuRect.setFillColor(sf::Color::Transparent);
	menuRect.setOutlineColor(sf::Color::White);
	menuRect.setOutlineThickness(2);
	checkStages();
}


void StageLoader::update(sf::RenderWindow& window,
	int& frame,
	std::map<std::string, sf::Texture>& textureMap,
	sf::Font& font,
	std::string& state,
	std::vector<Enemy>& enemies,
	int& stageLength,
	std::map<std::string, BulletPattern> patternMap
)
{
	const std::string folderName = "stages/";
	std::fstream fstr;

	// temp things for loading below
	std::string input, tempStr, tempKey;

	int enemyCount = 0, patternCount = 0;
	sf::Vector2f tempVec;
	std::vector<std::vector<std::string>> enemyPatterns(1);

	menuRect.setSize(sf::Vector2f(window.getSize().x/3*2, window.getSize().y/5*4));
	menuRect.setPosition(window.getSize().x/2-menuRect.getSize().x/2,
		window.getSize().y/2 - menuRect.getSize().y/2);

	addStage.setSize(sf::Vector2f(32, 32));
	addStage.setPosition(window.getSize().x / 2 + addStage.getSize().x / 2,
		menuRect.getPosition().y + menuRect.getSize().y);
	addStage.setOutlineThickness(1);
	addStage.setupSprite(textureMap["plusIcon"], 512, 512);

	if (addStage.checkMouse(window) && m1.consumeClick(frame, 20))
	{
		stageLength = 0;
		enemies.clear();
		state = "edit";
	}

	if (buttons.size() == 0)
	{
		for (std::string& stageName : stages)
			buttons.push_back(StageLoaderButton(textureMap,
				menuRect.getSize(),
				menuRect.getPosition(),
				stageName,
				font,
				buttons
			));
	}

	addStage.draw(window);
	window.draw(menuRect);
	for (StageLoaderButton& button : buttons)
	{
		if (button.getState() != "loader")
		{
			state = button.getState();
			button.setState("loader");
			fstr.open(folderName + "/" + button.getStageName() + ".stg");
			
			std::getline(fstr, input);
			stageLength = std::stoi(input);
			std::getline(fstr, input);

			for (int i = 0; i < input.size(); i++) // enemy start frames
			{
				if (input[i] != ',')
					tempStr.push_back(input[i]);
				else
				{
					enemies.push_back(Enemy(0, textureMap["enemy"]));
					enemies[enemies.size() - 1].setStartFrame(stoi(tempStr));
					tempStr.erase();
				}
			}

			std::getline(fstr, input);

			for (int i = 0; i < input.size(); i++) // paths
			{
				if (input[i] == ',')
				{
					tempVec.x = stod(tempStr);
					tempStr.erase();
				}
				else if (input[i] == ';')
					enemyCount++;
				else if (input[i] == ' ')
				{
					tempVec.y = stod(tempStr);
					enemies[enemyCount].pushToPath(tempVec);
					tempVec.x = -1;
					tempVec.y = -1;
					tempStr.erase();
				}
				else
					tempStr.push_back(input[i]);
			}

			tempStr.erase();
			enemyCount = 0;
			std::getline(fstr, input);

			for (int i = 0; i < input.size(); i++) // pattern strings
			{
				if (input[i] == ',')
				{
					enemyPatterns[enemyCount].push_back(tempStr);
					tempStr.erase();
				}
				else if (input[i] == ';')
				{
					enemyCount++;
					enemyPatterns.push_back(std::vector<std::string>()); // to make sure we stay in bounds.
				}
				else
					tempStr.push_back(input[i]);
			}

			enemyCount = 0;
			std:getline(fstr, input);

			for (int i = 0; i < input.size(); i++) // pattern starts
			{
				if (input[i] == ',')
				{
					tempKey = enemyPatterns[enemyCount][patternCount++];
					if (!patternMap.count(tempKey))
						continue;
					enemies[enemyCount].pushToPatterns(
						patternMap[tempKey], stoi(tempStr)
					);
					tempStr.erase();
				}
				else if (input[i] == ';')
				{
					enemyCount++;
					patternCount = 0;
				}
				else
					tempStr.push_back(input[i]);
			}
		}

		button.update(window, frame);
	}
	fstr.close();
}


void StageLoader::checkStages()
{
	stages.clear();
	buttons.clear();
	const std::string folderPath = "stages/";
	std::string fileName;
	std::fstream fstr;

	for (std::filesystem::directory_entry file :
	std::filesystem::directory_iterator(folderPath))
	{
		fileName = file.path().string().substr(folderPath.size(),
			file.path().string().find_first_of(".") - folderPath.size());
		fstr.open(folderPath + fileName + ".stg", std::ios::in);

		if (fstr.is_open())
			stages.push_back(fileName);
	}

	fstr.close();
}