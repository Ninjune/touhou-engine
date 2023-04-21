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
	frequency = frequencyIn;
	burstCount = burstCountIn;
	burstSize = burstSizeIn;
	burstSizeChange = burstSizeChangeIn;
	direction = directionIn;
	directionChange = directionChangeIn;
	spawnDirection = spawnDirection;
	spawnDirectionChange = spawnDirectionChangeIn;
	velocity = velocityIn;
	velocityChange = velocityChangeIn;
	bulletType = bulletTypeIn;
	name = nameIn;
}


void BulletPattern::update(std::vector<Bullet> bullets,
	int& stageFrame,
	sf::Vector2f origin,
	int fireFrame
)
{
	if (stageFrame >= fireFrame)
	{
		// bullets.push_back(); // HERE
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
*/