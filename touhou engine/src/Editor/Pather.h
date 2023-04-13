#pragma once
#include <SFML/Graphics.hpp>
#include "../Util/Keybind.h"
#include "Timeline.h"
#include "../Util/Path.h"
#include "../Entity/Enemy.h"
#include "Tool.h"

// this is the "stage editor".
class Pather
{
public:
	Pather(sf::RenderWindow& window, sf::Font& font, int screenWIn, int screenHIn);
	void update(sf::RenderWindow&, int& frame, sf::Texture& enemyTexture);
	void poll(sf::Event&);
	Path getSelectedEnemyPath();
private:
	Keybind upKey, downKey, m1, rKey, m2, backspace, delkey;
	std::vector<Keybind> numKeys;
	sf::VertexArray pathRender;
	int stageLength, SCREENHEIGHT, SCREENWIDTH;
	sf::Text inputText;
	std::string inputStr;
	std::vector<Tool> tools;
	Timeline timeline;
	sf::RectangleShape playableArea;
	sf::Vector2f patherToPlayable(sf::Vector2f point,
		sf::RectangleShape& playableArea, sf::RenderWindow& window);
	sf::Vector2f playableToPather(sf::Vector2f point,
		sf::RectangleShape& playableArea, sf::RenderWindow& window);
	sf::Vector2f mousePos;
	int selectedEnemyIndex;
	std::vector<Enemy> patherEnemies;
	std::vector<Bullet> patherBullets;
	sf::Texture selectionTexture, pencilTexture, duplicateTexture;
	bool selecting, canDuplicate;
	void draw(sf::RenderWindow& window, int& frame);
};

