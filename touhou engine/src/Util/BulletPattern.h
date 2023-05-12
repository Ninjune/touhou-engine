#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "../Entity/Bullet.h"


class BulletPattern
{
public:
	BulletPattern();
	BulletPattern(std::map<std::string, float> options,
		std::string bulletType,
		std::string name
	);
	void update(sf::RenderWindow& window,
		std::map<std::string, sf::Texture>& textureMap,
		std::vector<std::vector<std::vector<Bullet>>>& bullets,
		int& stageFrame,
		sf::Vector2f origin,
		int fireFrame,
		std::vector<sf::Vector2f> path,
		sf::RectangleShape playableArea,
		int startFrame,
		sf::Vector2f playerPos,
		int deadFrame,
		bool& inBounds,
		bool pather = false
	);
	std::string getName();
	// index += in
	void modifyIndex(int in);
	std::map<std::string, float> getOptions();
	void setOptions(std::map<std::string, float> options);
private:
	sf::Vector2f playableToPather(sf::Vector2f point,
		sf::RenderWindow& window,
		sf::RectangleShape& playableArea
	);
	sf::Vector2f origin;
	int bulletsIndex;
	std::map<std::string, float> options;
	std::string bulletType;
	std::string name;
	std::vector<int> reservedBullets;
	bool changeWithEnemy;
};