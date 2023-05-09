#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Bullet.h"
#include "Enemy.h"
#pragma once

class Player : public Character
{
public:
	Player(sf::RenderWindow& window,
		std::map<std::string, sf::Texture>& textureMap
	);
	void updateSprite(sf::RenderWindow& window,
		int frame,
		std::vector<Bullet>& playerBullets,
		std::vector<Enemy>& enemies,
		std::vector<std::vector<std::vector<Bullet>>>& bullets,
		std::map<std::string, sf::Texture>& textureMap
	);
private:
	sf::Texture hitboxTexture;
	sf::RectangleShape hitbox;
	int leftPhase, rightPhase, life, immunity;
	int speed, shootSpeed; // shootSpeed is amount of shots in a second.
	int shootCooldown; // frames til can shoot again
	std::map<sf::Keyboard::Key, sf::Vector2f> movementKeyMap;
	std::vector<sf::Keyboard::Key> movementKeys;
	sf::Keyboard::Key shootKey;
	std::vector<sf::Sprite> lifeStars;
};