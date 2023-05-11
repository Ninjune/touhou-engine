#pragma once
#include <SFML/Graphics.hpp>
#include "../Util/Keybind.h"
#include "Timeline.h"
#include "../Util/Path.h"
#include "../Entity/Enemy.h"
#include "Tool.h"
#include "Bullet Pattern Menu/BulletPatternMenu.h"
#include "Bullet Pattern Menu/PatternChanger.h"
#include <fstream>
// this is the "stage editor".


class Pather
{
public:
	Pather(sf::RenderWindow& window,
		sf::Font& font,
		std::map<std::string, sf::Texture>& textureMap,
		int screenWIn,
		int screenHIn,
		std::map<std::string, BulletPattern>& patterns
	);
	void update(sf::RenderWindow&,
		int& frame,
		std::map<std::string, sf::Texture>& textureMap,
		std::map<std::string, BulletPattern>& patterns,
		std::vector<Enemy>& enemies,
		int stageLength = -1
	);
	void poll(sf::Event&, int stageLength);
	Path getSelectedEnemyPath();
private:
	Keybind upKey, downKey, m1, rKey, m2, backspace, delkey, eKey;
	std::vector<Keybind> numKeys;
	sf::VertexArray pathRender;
	int SCREENHEIGHT, SCREENWIDTH;
	sf::Text inputText;
	std::string inputStr;
	std::vector<Tool> tools;
	Timeline timeline;
	BulletPatternMenu bulletPatternMenu;
	sf::RectangleShape playableArea;
	std::vector<Enemy> patherEnemies;
	sf::Vector2f mousePos;
	int selectedEnemyIndex;
	std::vector<std::vector<std::vector<Bullet>>> patherBullets;
	bool selecting, canDuplicate, enterStageName, loaded, patternChanging;
	PatternChanger patternChanger;

	void draw(std::map<std::string, sf::Texture>&,
		sf::RenderWindow& window,
		int& frame,
		std::map<std::string, BulletPattern>& patterns
	);
	void prompt(sf::RenderWindow& window, std::string input);
	bool selectTool(sf::RenderWindow& window,
		int& frame,
		std::vector<std::vector<std::vector<Bullet>>>& bullets // to move bullets
	);
	bool drawTool(sf::RenderWindow& window,
		int& frame
	);
	sf::Vector2f patherToPlayable(sf::Vector2f point,
		sf::RectangleShape& playableArea, sf::RenderWindow& window);
	sf::Vector2f playableToPather(sf::Vector2f point,
		sf::RectangleShape& playableArea, sf::RenderWindow& window);
};

