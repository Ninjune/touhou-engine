#include "StageLoader.h"


StageLoader::StageLoader()
{
	const std::string folderName = "stages";
	std::string fileName;
	std::fstream fstr;
	menuRect.setFillColor(sf::Color::Transparent);
	menuRect.setOutlineColor(sf::Color::White);
	menuRect.setOutlineThickness(2);

	for (std::filesystem::directory_entry file :
	std::filesystem::directory_iterator(folderName))
	{
		fileName = file.path().string().substr(folderName.size(),
			file.path().string().find_first_of(".") - folderName.size() - 4);
		fstr.open(folderName + "/" + fileName + ".stg", std::ios::in);

		if (fstr.is_open())
			stages.push_back(fileName);
	}
}


void StageLoader::update(sf::RenderWindow& window,
	int& frame,
	std::map<std::string, sf::Texture>& textureMap,
	sf::Font& font,
	std::string& state,
	std::vector<Enemy>& enemies,
	int& stageLength
)
{
	const std::string folderName = "stages/";
	std::fstream fstr;

	// temp things for loading below
	std::string input, tempStr, tempStr2;
	int enemyCounter = 0;
	sf::Vector2f tempVec;
	bool onX = true;
	BulletPattern tempBP;

	if (buttons.size() == 0)
	{
		for (std::string& stageName : stages)
			buttons.push_back(StageLoaderButton(textureMap,
				window.getSize(),
				stageName,
				font
			));
	}

	menuRect.setSize(sf::Vector2f(window.getSize().x/3*2, window.getSize().y/5*4));
	menuRect.setPosition(window.getSize().x/2-menuRect.getSize().x/2,
		window.getSize().y/2 - menuRect.getSize().y/2);

	window.draw(menuRect);
	for (StageLoaderButton& button : buttons)
	{
		if (button.getState() != "loader")
		{
			state = button.getState();
			fstr.open(folderName + "/" + button.getStageName() + ".stg");
			
			std::getline(fstr, input);
			stageLength = std::stoi(input);
			std::getline(fstr, input);

			for (int i = 0; i < input.size(); i++)
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

			for (int i = 0; i < input.size(); i++)
			{
				if (input[i] == ',')
				{
					if (onX)
						tempVec.x = stod(tempStr);
					else
						tempVec.y = stod(tempStr);
					onX = !onX;
				}
				else if (input[i] == ';')
				{
					enemies[enemyCounter++].pushToPath(tempVec);
					onX = true;
				}
				else
					tempStr.push_back(input[i]);
			}

			enemyCounter = 0;
			std::getline(fstr, input);

			for (int i = 0; i < input.size(); i++)
			{
				if(input[i] == ',')
				else if (input[i])
				// HERE
			}
		}

		button.update(window, frame);
	}
}