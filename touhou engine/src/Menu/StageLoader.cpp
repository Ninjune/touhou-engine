#include "StageLoader.h"


StageLoader::StageLoader()
{
	const std::string folderName = "stages/";
	std::string fileName;
	std::fstream fstr;
	menuRect.setFillColor(sf::Color::Transparent);
	menuRect.setOutlineColor(sf::Color::White);
	menuRect.setOutlineThickness(2);

	for (std::filesystem::directory_entry file :
	std::filesystem::directory_iterator(folderName))
	{
		fileName = file.path().string().substr(folderName.size(),
			file.path().string().find_first_of(".") - folderName.size());
		fstr.open(folderName + "/" + fileName + ".stg", std::ios::in);
		// HERE 
	}
}


void StageLoader::update(sf::RenderWindow& window,
	int& frame,
	sf::Font& font
)
{
	menuRect.setSize(sf::Vector2f(window.getSize().x/3*2, window.getSize().y/5*4));
	menuRect.setPosition(window.getSize().x/2-menuRect.getSize().x/2,
		window.getSize().y/2 - menuRect.getSize().y/2);

	window.draw(menuRect);
	for (StageLoaderButton& button : buttons)
		button.update(window, frame);
}