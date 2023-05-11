#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Bullet.h"
#include "../Util/Path.h"
#include "../Util/BulletPattern.h"
#pragma once

class Enemy : public Character
{
public:
	Enemy(int type,
		sf::Texture& texture
	);
	void updateSprite(std::map<std::string, sf::Texture>& textureMap,
		sf::RenderWindow& window,
		int frame,
		std::vector<std::vector<std::vector<Bullet>>>& bullets,
		int currentFrame,
		bool pather = true
	);
	void setTexture(sf::Texture& texture);
	void setType(int type);
	void setPathSpeed(int);
	void setPlayableArea(sf::RectangleShape in);
	void setPosition(sf::Vector2f pos,
		std::vector<std::vector<std::vector<Bullet>>>& bullets
	);
	void setStartFrame(int);
	Path getPath();
	std::vector<std::string> getPatterns();
	void setPatterns(std::map<std::string, BulletPattern>& patternMap,
		std::vector<std::string> patternsToAdd,
		std::vector<int> startTimes
	); // used for setting when playing game
	std::vector<int> getStartTimes();
	int getStartFrame();
	void clearPath();
	void pushToPath(sf::Vector2f point);
	void pushToPatterns(BulletPattern pattern, int startTime);
	void eraseFromPatterns(int currentFrame);
private:
	void init(int type, 
		sf::Texture& texture
	);
	int movementFrame, point, enemyType; // type 0-3: small sprites, 
	float moveX, moveY;
	int startFrame;
	void moveToPoint(sf::Vector2f, int framesToTake);
	sf::Clock timer;
	std::vector<BulletPattern> patterns;
	std::vector<int> patternStartTimes;
	Path path;
	sf::RectangleShape playableArea;
	sf::Vector2f playableToPather(sf::Vector2f point, sf::RenderWindow& window);
};