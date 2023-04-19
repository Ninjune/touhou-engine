#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Bullet.h"
#include "../Util/Path.h"
#include "../Util/BulletPattern.h"
#pragma once

class Enemy : public Character
{
public:
	Enemy(int type, sf::Texture& texture);
	void updateSprite(sf::RenderWindow& window, int frame, std::vector<Bullet>& bullets, int currentFrame = -1);
	bool isPathFine();
	void setTexture(sf::Texture& texture);
	void setType(int type);
	void setPathSpeed(int);
	void setPlayableArea(sf::RectangleShape in);
	void setPosition(sf::Vector2f pos);
	void setStartFrame(int);
	Path getPath();
	std::vector<BulletPattern> getPatterns();
	int getStartFrame();
	void clearPath();
	void pushToPath(sf::Vector2f point);
	void pushToPatterns(BulletPattern pattern);
	void eraseFromPatterns(std::string name);
private:
	void init(int type, sf::Texture& texture);
	int movementFrame, point, enemyType; // type 0-3: small sprites, 
	float moveX, moveY;
	int startFrame;
	void moveToPoint(sf::Vector2f, int framesToTake);
	std::vector<Bullet> bullets;
	bool pathFine;
	sf::Clock timer;
	std::vector<BulletPattern> patterns;
	Path path;
	sf::RectangleShape playableArea;
	sf::Vector2f playableToPather(sf::Vector2f point, sf::RenderWindow& window); // maybe add some like static global function for this if can
};