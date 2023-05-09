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
	directionChange = directionChangeIn; // change in direction per burst
	spawnDirection = spawnDirectionIn; // place to spawn the bullets based on a radius. will spawn in a circle around the target 0-360
	spawnDirectionChange = spawnDirectionChangeIn; // change between burst
	velocity = velocityIn; // speed of bullet
	velocityChange = velocityChangeIn; // velocity change per burst
	bulletType = bulletTypeIn; // type, TBA
	name = nameIn; // pattern name, set to file name

	bulletsIndex = -1;
	changeWithEnemy = false;
}


void BulletPattern::update(sf::RenderWindow& window,
	std::map<std::string, sf::Texture>& textureMap,
	std::vector<std::vector<std::vector<Bullet>>>& bullets, // first for each pattern, second for each burst, third for each bullet
	int& stageFrame,
	sf::Vector2f enemyPos,
	int fireFrame
)
{
	if (bulletsIndex == -1)
	{
		bullets.push_back({}); // push an empty array
		bulletsIndex = bullets.size() - 1;

		for (unsigned int i = 0; i < burstCount; i++)
		{
			bullets[bulletsIndex].push_back({});
			for (unsigned int j = 0; j < burstSize + (burstSizeChange * i); j++)
			{
				bullets[bulletsIndex][i].push_back(Bullet(textureMap,
					origin,
					direction + (directionChange * i),
					spawnDirection + (spawnDirectionChange * i),
					velocity + (velocityChange * i),
					bulletType
				));
			}
		}
	}
	if (changeWithEnemy || origin.x <= -1)
	{
		origin = enemyPos;
		changeWithEnemy = true;
	}
	// enemy origin will change every time simulateFrame is called when it should only be changed on burst for the specific burst

	// burst
	int burstNum = (stageFrame - fireFrame) / frequency + 1;
	if (burstNum < 1 || stageFrame < fireFrame)
		return;

	if (burstNum > burstCount)
		burstNum = burstCount;

	for (unsigned int i = 0; i < burstNum; i++)
	{
		for (unsigned int j = 0; j < bullets[bulletsIndex][i].size(); j++)
		{
			bullets[bulletsIndex][i][j].simulateFrames(window, (stageFrame - fireFrame + 1) - frequency * i, origin);
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

The file name will be the name, excluding .txt

For origin: if either is a negative number then require Enemy to give it's origin as it will be relative to enemy.
For direction: if negative then give a direction towards player
*/