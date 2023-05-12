#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Bullet.h"
#include "Enemy.h"
#include "../Util/Keybind.h"
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
	void reset();
	int getLife();
private:
	sf::Texture hitboxTexture;
	sf::RectangleShape hitbox;
	int leftPhase, rightPhase;
	int speed, shootCD; // shootSpeed is amount of shots in a second.
	std::map<sf::Keyboard::Key, sf::Vector2f> movementKeyMap;
	std::vector<sf::Keyboard::Key> movementKeys;
	Keybind Z;
	std::vector<sf::Sprite> lifeStars;
};