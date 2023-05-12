#include "BulletPattern.h"

BulletPattern::BulletPattern() // dont use
{

}


BulletPattern::BulletPattern(std::map<std::string, float> optionsIn,
	std::string bulletTypeIn,
	std::string nameIn
)
{
	options = optionsIn;
	
	bulletType = bulletTypeIn; // type, texture basically
	name = nameIn; // pattern name, set to file name

	bulletsIndex = -1;
	changeWithEnemy = false;
}


void BulletPattern::update(sf::RenderWindow& window,
	std::map<std::string, sf::Texture>& textureMap,
	std::vector<std::vector<std::vector<Bullet>>>& bullets, // first for each pattern, second for each burst, third for each bullet
	int& stageFrame,
	sf::Vector2f enemyPos,
	int fireFrame,
	std::vector<sf::Vector2f> path,
	sf::RectangleShape playableArea,
	int startFrame,
	sf::Vector2f playerPos,
	int deadFrame,
	bool& inBounds,
	bool pather
)
{
	int frequency = options["frequency"], // frequency of bursts
		burstCount = options["burstCount"], // amount of bursts
		burstSize = options["burstSize"], // amount of bullets per burst
		burstSizeChange = options["burstSizeChange"], // change in burstSize per burst
		direction = options["direction"], // direction in which bullet goes 0-360
		directionChange = options["directionChange"], // change in direction per burst
		spawnDirection = options["spawnDirection"], // place to spawn the bullets based on a radius. will spawn in a circle around the target 0-360
		spawnDirectionChange = options["spawnDirectionChange"]; // change between burst
	float velocity = options["velocity"], // speed of bullet
		velocityChange = options["velocityChange"]; // velocity change per burst

	if (bulletsIndex == -1 || bulletsIndex >= bullets.size())
	{
		bullets.push_back({}); // push an empty array
		bulletsIndex = bullets.size() - 1;

		for (unsigned int i = 0; i < burstCount; i++)
		{
			bullets[bulletsIndex].push_back({});
			for (unsigned int j = 0; j < burstSize + (burstSizeChange * i); j++)
			{
				bullets[bulletsIndex][i].push_back(Bullet(textureMap,
					sf::Vector2f(options["originX"], options["originY"]),
					direction + (directionChange * j),
					spawnDirection + (spawnDirectionChange * i),
					velocity + (velocityChange * i),
					bulletType
				));
			}
		}
	}

	// burst
	inBounds = true;
	int burstNum = ((deadFrame == -1 ? stageFrame : deadFrame) - fireFrame) / frequency + 1;
	if (burstNum < 1 || stageFrame < fireFrame)
		return;

	if (burstNum >= burstCount)
	{
		burstNum = burstCount;
		inBounds = false;
	}

	for (unsigned int i = 0; i < burstNum; i++)
	{
		if (fireFrame - startFrame + frequency * i >= path.size())
			continue;
		for (unsigned int j = 0; j < bullets[bulletsIndex][i].size(); j++)
		{
			bullets[bulletsIndex][i][j].simulateFrames(window,
				(stageFrame - fireFrame + 1) - frequency * i,
				pather ? playableToPather(path[fireFrame-startFrame + frequency*i],
					window, playableArea) :
					path[fireFrame-startFrame + frequency*i], playerPos, inBounds
			);
		}
	}
}


std::string BulletPattern::getName()
{
	return name;
}


sf::Vector2f BulletPattern::playableToPather(sf::Vector2f point,
	sf::RenderWindow& window,
	sf::RectangleShape& playableArea
)
{
	return sf::Vector2f(
		(point.x - playableArea.getPosition().x) /
		window.getSize().x * playableArea.getSize().x +
		playableArea.getPosition().x,
		(point.y - playableArea.getPosition().y) /
		window.getSize().y * playableArea.getSize().y +
		playableArea.getPosition().y
	);
}


std::map<std::string, float> BulletPattern::getOptions()
{
	return options;
}


 void BulletPattern::setOptions(std::map<std::string, float> in)
{
	options = in;
	std::fstream fstr;

	fstr.open("patterns/" + name + ".txt", std::ios::out);

	fstr << options["originX"] << " " << options["originY"] << " " <<
		options["frequency"] << " " << options["burstCount"] << " " << 
		options["burstSize"] << " " << options["burstSizeChange"] << " " << 
		options["direction"] << " " << options["directionChange"] << " " <<
		options["spawnDirection"] << " " << options["spawnDirectionChange"] <<
		" " << options["velocity"] << " " << options["velocityChange"] << " " <<
		bulletType; // fun

	fstr.close();
}


 void BulletPattern::modifyIndex(int in)
 {
	 bulletsIndex += in;
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