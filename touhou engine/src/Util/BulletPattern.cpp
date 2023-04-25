#include "BulletPattern.h"


BulletPattern::BulletPattern()
{

}


BulletPattern::BulletPattern(sf::Vector2f originIn,
	int frequencyIn,
	int burstCountIn,
	int burstSizeIn,
	int burstSizeChangeIn,
	int directionIn,
	int directionChangeIn,
	int spawnDirectionIn,
	int spawnDirectionChangeIn,
	double velocityIn,
	double velocityChangeIn,
	std::string bulletTypeIn,
	std::string nameIn
)
{
	origin = originIn;
	frequency = frequencyIn; // frequency of bursts (collection of bullets)
	burstCount = burstCountIn; // amount of bursts
	burstSize = burstSizeIn; // amount of bullets per burst
	burstSizeChange = burstSizeChangeIn; // change in burstSize per burst
	direction = directionIn; // direction in which bullet goes 0-360
	directionChange = directionChangeIn; // change in direction in burst
	spawnDirection = spawnDirectionIn; // place to spawn the bullets based on a radius. will spawn in a circle around the target 0-360
	spawnDirectionChange = spawnDirectionChangeIn; // change between burst
	velocity = velocityIn; // speed of bullet
	velocityChange = velocityChangeIn; // velocity change per burst
	bulletType = bulletTypeIn; // type, TBA
	name = nameIn; // pattern name, set to file name
}


void BulletPattern::update(std::map<std::string, sf::Texture>& textureMap,
	std::vector<Bullet> bullets,
	int& stageFrame,
	sf::Vector2f origin,
	int fireFrame
)
{
	if (reservedBullets.empty())
	{
		for (unsigned int i = bullets.size() - 1; i < bullets.size() - 1 + burstSize * burstCount; i++)
		{
			reservedBullets.push_back(i);
			bullets.push_back(Bullet());
		}
	}

	if (stageFrame >= fireFrame)
	{
		for (unsigned int i = 0; i < (stageFrame - fireFrame) / frequency; i++)
		{
			// burst
			// HERE
		}
	}
}


std::string BulletPattern::getName()
{
	return name;
}
/*
FILE FORMAT:
originX originY frequency burstCount burstSize burstSizeChange direction (just continued no \n)
directionChange spawnDirection spawnDirectionChange velocity velocityChange
bulletType

The file name will be the name. Exclude .txt

For origin: if either is a negative number then require Enemy to give it's origin as it will be relative to enemy.
For direction: if negative then give a direction towards player
*/